#include <stdlib.h>
#include <stdio.h>
#include <scheduler_s.h>
#include <prgenerator.h>
#include <pthread.h>
#include <lnklist_s.h>
#include <pcb_s.h>
#include <err.h>
#include <executor.h>
#include <assert.h>
#include <loader.h>

# define SCHEDULER_NULL_PROCESS_RR_FREQ 4

# define SCHEDULER_BASE_PROCESS_RR_FREQ 10



void sched_AddToProcessQueue(sched_basic_t * sched, pcb_t * new_pcb){
    pthread_mutex_lock(sched->sched_mutex_list);
    new_pcb->status = PR_READY;
    i_sched_in(&sched->scheduler_queue_iface_object, new_pcb);
    pthread_mutex_unlock(sched->sched_mutex_list);
}

void sched_execute(sched_basic_t * sched){
    for (int i = 0; i < sched->coreControl_arr_len; i++)
    {
        struct sched_cpuThreadControl * core_c=sched->coreControl_array[i];
        if(core_c->current_cpu_SchedStatus == NULL_PROCESS_RUN){
            sched_Scheduler_Dispatcher(sched, core_c);
        }else if(MY_SCHED_PRIO_RR == sched->current_politika || MY_SCHED_RR == sched->current_politika){
            if(core_c->cur_process_quantum <= 0){
                sched_Scheduler_Dispatcher(sched, core_c);
            }else{
                core_c->cur_process_quantum -= sched->timer->tmr_tick_freq;
            }
 
        }
    }   
}

void sched_Scheduler_Dispatcher(sched_basic_t * sched, struct sched_cpuThreadControl * thr_control){
    printf("Sheduler Activated:\n");
    pcb_t * next_process, * old_process;
    
    // CONSEGUIR: NEXT y OLD process
    // OLD PROCESS:
    __sched_updateRunning_pcb(thr_control);
    old_process = thr_control->current_running_process;
    printf("- Old: PID = %d ; prio = %d ; status = %d\n", old_process->pid, old_process->curr_prio, old_process->status);
    
    next_process = __sched_getNextProcessAndCleanQueue(sched, thr_control);

    
    // Si el old process debe terminar y no hay otro proceso que lo pueda remplazar
    if(next_process == NULL){
        if(old_process != &(sched->null_process) && old_process->status == PR_FINI){
            next_process = &sched->null_process; // Next Process sera el null process
            thr_control->cur_process_quantum = SCHEDULER_NULL_PROCESS_RR_FREQ;
        }
    }else{
        thr_control->cur_process_quantum = SCHEDULER_BASE_PROCESS_RR_FREQ;
    }

    // HACER CAMBIO DE CONTEXTO:
    // Si el next process esta disponible, entonces cambiar el contexto
    if (next_process != NULL){
        thr_control->current_running_process = next_process;
        printf("- Next: PID = %d ; prio = %d ; status = %d\n",next_process->pid, next_process->curr_prio, next_process->status);

        disp_Dispatch(old_process, next_process, thr_control->core_hari_s);

        // OLD_PROCESS ez bada NULL_PROCESS edo Amaitu behar duen prozesua
        if(thr_control->current_cpu_SchedStatus != NULL_PROCESS_RUN){
            if(old_process->status == PR_FINI){ // 
                disp_Terminate(thr_control->core_hari_s, old_process); 
            }else{
                sched_AddToProcessQueue(sched, old_process);
                old_process->status = PR_READY;
            }
        }
    }else{
        printf("- Next: Keep last");
    }
    printf(" - comparation %d and %d\n", (unsigned int) next_process, (unsigned int) &sched->null_process);

    // NUEVO ESTADO DE SCHEDULER
    // Configurar nuevo estado de scheduler:
    if(next_process != NULL){
        if(next_process == &sched->null_process){
            thr_control->current_cpu_SchedStatus = NULL_PROCESS_RUN;
            printf("NULL PROCESS\n");
        }else{
            thr_control->current_cpu_SchedStatus = PROCESS_RUN;
            printf("RUNNING PROCESS\n");
        }
    }
    
    printf("current sheduler status: %d", sched->current_sched_status);
}

/**
 * @brief Funtzioa exekutatzen den momentuan "thr_control"-ean exekutatzen ari den 
 * prozesuaren PID-a eguneratzen du. Prozesua exekuzioan dagoela suposatzen da.
 * 
 * @pre thr_control->current_running_process ezin da NULL izan
 * @param thr_control 
 * @return int 
 */
int __sched_updateRunning_pcb(struct sched_cpuThreadControl * thr_control){
    assert(thr_control != NULL);
    assert(thr_control->current_running_process != NULL);

    thr_control->current_running_process->status = PR_EXEC;
    
    if(thr_control->core_hari_s->IR == EXIT_CODE ){
        thr_control->current_running_process->status = PR_FINI;
    }
}


pcb_t * __sched_getNextProcessAndCleanQueue(sched_basic_t * sched, struct sched_cpuThreadControl * thread_control){
    // NEXT PROCESS:
    // Encontrar next process valido --> finalizar los procesos que han terminado
    int keep_searching_processes = 1;
    pcb_t * next_process;
    while (keep_searching_processes){
        next_process = sched_GetNextFromProcessQueue(sched);
        if(next_process != NULL && next_process->status == PR_FINI)
            disp_Terminate(thread_control->core_hari_s, next_process);
        else
            keep_searching_processes = 0;
    }
    return next_process;
}

int sched_cpuThreadControl_init(struct sched_cpuThreadControl* thr_ctrl, core_hari_s *core_hari_s, pcb_t *cur_process, int cur_process_quantum, int current_cpu_SchedStatus)
{
    thr_ctrl->core_hari_s = core_hari_s;
    thr_ctrl->cur_process_quantum = cur_process_quantum;
    thr_ctrl->current_cpu_SchedStatus = current_cpu_SchedStatus;
    thr_ctrl->current_running_process = cur_process;
    return 0;
}

pcb_t * sched_GetNextFromProcessQueue(sched_basic_t * sched){
    pcb_t * next_process;
    pthread_mutex_lock(sched->sched_mutex_list);
    next_process = i_sched_out(&sched->scheduler_queue_iface_object);
    pthread_mutex_unlock(sched->sched_mutex_list);
    return next_process;
}

int sched_queueIsEmpty(sched_basic_t * sched){
    return i_sched_peek(&sched->scheduler_queue_iface_object) == NULL;
}

void disp_Dispatch(pcb_t * old_pcb, pcb_t * new_pcb, struct core_hari_s * cpu_core_hari){
    printf("DISPATHCHER: SWITCH --> OLD:");
    if(old_pcb != NULL){
        printf("%d", old_pcb->pid);
        __disp_unload(cpu_core_hari, old_pcb);
        }
    else printf("NULL");

    printf(" - NEW:");
    if(new_pcb != NULL) {
        printf("%d",new_pcb->pid);
        __disp_load(cpu_core_hari, new_pcb);
    } else printf("NULL");
    printf("\n");
}


void disp_Terminate(struct core_hari_s * cpu_core_hari, pcb_t * pcb){
    printf("DISPATHCHER: Process with %d PID terminated", pcb->pid);
    mmu_free(&cpu_core_hari->mem_manag_unit, pcb->memo_m.pgb);
    pcb_destroy(pcb);
}

int __sched_threadControlArray_init(sched_basic_t * sched,struct cpu_s  my_cpu_arr[], int cpu_kop);
void sched_init(sched_basic_t * sched, clk_timer_s * timer, struct cpu_s  my_cpu_arr[], int cpu_kop, int SCHED_POLITIKA){ //TODO: Hay que configurar el inicilizador para que admita diferentes politicas
 
    // Schedulerraren mutex-a hasieratu
    sched->sched_mutex_list = (pthread_mutex_t * ) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(sched->sched_mutex_list, NULL);

    // Prozesu nulua
    pcb_init(&sched->null_process, -1, 0);
    sched->current_running_process = &(sched->null_process);
    sched->current_sched_status = NULL_PROCESS_RUN;
    loadNullPCB(&(my_cpu_arr[0].core_arr[0]->hari_arr[0]->mem_manag_unit), &(sched->null_process));

    sched->current_politika = SCHED_POLITIKA;

    i_schedStartup(&(sched->scheduler_queue_iface_object), SCHED_POLITIKA);
    sched->timer = timer;
    sched->default_process_quantum = timer->tick_count;

    __sched_threadControlArray_init(sched, my_cpu_arr, cpu_kop);
}

void __disp_load(core_hari_s *cpu_core_hari, pcb_t *pcb)
{
    cpu_core_hari->PTBR = pcb->memo_m.pgb;
    cpu_core_hari->IR = pcb->s.IR;
    cpu_core_hari->PC = pcb->s.PC;
    cpu_core_hari->cc = pcb->s.cc;
    for(int i = 0; i < CPU_HARI_REG_KOP; i++){
        cpu_core_hari->R[i] = pcb->s.R[i];
    }  
    pcb->status = PR_EXEC;

}
void __disp_unload(core_hari_s *cpu_core_hari, pcb_t *pcb)
{
    pcb->s.IR = cpu_core_hari->IR;
    pcb->s.PC = cpu_core_hari->PC;
    cpu_core_hari->cc = pcb->s.cc;
    for(int i = 0; i < CPU_HARI_REG_KOP; i++){
        pcb->s.R[i] = cpu_core_hari->R[i];
    }
    
}


/**
 * @brief Sheduler-aren ThreadControlArray-a hasieratzen du. Thread control objektuak sortuz eta hauek dagokien CPU-arekin hasieratuz.
 * Une honetatik aurrera, Schedulerra eta Hardware egitura lotuta egongo dira.
 * Gainera CPU-ak null prozesuarekin hasieratzen ditu.
 * 
 * @param sched lotu nahi den schedulerra
 * @param my_cpu_arr Lotu nahi den CPU array egitura
 * @param cpu_kop CPU array-aren luzeera
 * @return int errore kodea
 */
int __sched_threadControlArray_init(sched_basic_t *sched, struct cpu_s my_cpu_arr[], int cpu_kop)
{
    // CPU guztien hari kopuruak batu:
    int cpu_hari_kop = 0;
    int curr_h_kop;
    for(int i = 0; i < cpu_kop; i++){
        get_hari_kop(&my_cpu_arr[i], &curr_h_kop);
        cpu_hari_kop += curr_h_kop;
    }
    
    sched->coreControl_arr_len = cpu_hari_kop;

    // Thread control objektuak sortu eta hasieratu.
    sched->coreControl_array = malloc(sizeof(struct sched_cpuThreadControl *)*cpu_hari_kop);
    int next_thread_ctrl_index = 0;
    for(int i = 0; i < cpu_kop; i++){
        int current_core_kop = my_cpu_arr[i].core_kant;
        struct core_s ** current_core_arr = my_cpu_arr[i].core_arr;
        for(int j = 0; j < current_core_kop; j++){
            int current_hari_kop = current_core_arr[j]->hari_kant;
            for(int k = 0 ; k < current_hari_kop; k++) {
                sched->coreControl_array[next_thread_ctrl_index] = malloc(sizeof(struct sched_cpuThreadControl));
                sched_cpuThreadControl_init(
                    sched->coreControl_array[next_thread_ctrl_index],
                    current_core_arr[j]->hari_arr[k],
                    &(sched->null_process),
                    SCHEDULER_NULL_PROCESS_RR_FREQ,
                    NULL_PROCESS_RUN
                );
                __disp_load(current_core_arr[j]->hari_arr[k], &(sched->null_process));
                next_thread_ctrl_index++;
            }
        }
    }
}

/**
 * @brief Scheduler baten thread control array-a suntsitzen du. Thread control 
 * elementu guztien free() eginez. Metodo pribatua da.
 * 
 * @param sched Target shecduler-aren helbidea
 * @return int 
 */
int __sched_threadControlArray_destroy(sched_basic_t *sched){
    for( int i = 0; sched->coreControl_arr_len; i++){
        free(sched->coreControl_array[i]);
    }
}


void sched_print_sched_State(sched_basic_t * sched){
    
    //TODO: ADD PRINT METHOD in scheduler iterface
    pthread_mutex_lock(sched->sched_mutex_list);

    i_sched_print(&sched->scheduler_queue_iface_object);

    printf("- Current process = %d \n", sched->current_running_process->pid);
    printf("  - Process Status = %d \n", sched->current_running_process->status);
    printf("  - Process prio = %d \n", sched->current_running_process->curr_prio);
    printf("- Sched_status = %d \n", sched->current_sched_status);
    printf("- null_process = %d \n", sched->null_process.pid);
    printf("- timer = %d \n", sched->default_process_quantum);
    printf("- politika = %d \n", sched->current_politika);

    pthread_mutex_unlock(sched->sched_mutex_list);
    
}

