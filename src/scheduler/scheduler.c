#include <stdlib.h>
#include <stdio.h>
#include <scheduler_s.h>
#include <prgenerator.h>
#include <pthread.h>
#include <lnklist_s.h>
#include <pcb_s.h>
#include <err.h>

# define SCHEDULER_NULL_PROCESS_RR_FREQ 4
void disp_Terminate(pcb_t * pcb);


void sched_AddToProcessQueue(sched_basic_t * sched, pcb_t * new_pcb){
    pthread_mutex_lock(sched->sched_mutex_list);
    i_sched_in(&sched->scheduler_queue_iface_object, new_pcb);
    pthread_mutex_unlock(sched->sched_mutex_list);
}

void sched_Scheduler_Dispatcher(sched_basic_t * sched){
    printf("Sheduler Activated:\n");
    pcb_t * next_process, * old_process;
    
    // CONSEGUIR: NEXT y OLD process
    // OLD PROCESS:
    old_process = sched->current_running_process;
    printf("- Old: PID = %d ; prio = %d ; status = %d\n", old_process->pid, old_process->curr_prio, old_process->status);
    // NEXT PROCESS:
    // Encontrar next process valido --> finalizar los procesos que han terminado
    int keep_searching_processes = 1;
    while (keep_searching_processes){
        next_process = sched_GetNextFromProcessQueue(sched);
        if(next_process != NULL && next_process->status == PR_FINI)
            disp_Terminate(next_process);
        else
            keep_searching_processes = 0;
    }
    // Si el old process debe terminar y no hay otro proceso que lo pueda remplazar
    if(old_process->status == PR_FINI && next_process == NULL){
        next_process = &sched->null_process; // Next Process sera el null process
    }

    // HACER CAMBIO DE CONTEXTO:
    // Si el next process esta disponible, entonces cambiar el contexto
    if (next_process != NULL){
        sched->current_running_process = next_process;
        printf("- Next: PID = %d ; prio = %d ; status = %d\n",next_process->pid, next_process->curr_prio, next_process->status);

        disp_Dispatch(old_process, next_process);

        // OLD_PROCESS ez bada NULL_PROCESS edo Amaitu behar duen prozesua
        if(sched->current_sched_status != NULL_PROCESS_RUN){
            if(old_process->status == PR_FINI){
                disp_Terminate(old_process);
            }else{
                sched_AddToProcessQueue(sched, old_process);
            }
        }
        
    }else{
        printf("- Next: Keep last");
    }
    printf(" - comparation %d and %d\n", next_process, &sched->null_process);

    // NUEVO ESTADO DE SCHEDULER
    // Configurar nuevo estado de scheduler:
    
    if(next_process != NULL){
        if(next_process == &sched->null_process){
            sched->current_sched_status = NULL_PROCESS_RUN;
            printf("NULL PROCESS\n");
        }else{
            sched->current_sched_status = PROCESS_RUN;
            printf("RUNNING PROCESS\n");
        }
    }
    
    printf("current sheduler status: %d", sched->current_sched_status);
    // Configurar siguiente llamada de scheduler:
    switch (sched->current_politika)
    {
    //Quantum konstante bidez
    case MY_SCHED_PRIO_RR:
    case MY_SCHED_RR:
        if(sched->current_sched_status == NULL_PROCESS_RUN){
            clk_change_timer_tickFreq(sched->timer, SCHEDULER_NULL_PROCESS_RR_FREQ, 1);
        }else{
            clk_change_timer_tickFreq(sched->timer, sched->default_process_quantum, 0);
        }
        break;
    
    //Gertaera bidez
    case MY_SCHED_PRIO_FIFO:
    case MY_SCHED_FIFO:
    default:
        break;
    }
   
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

void disp_Dispatch(pcb_t * old_pcb, pcb_t * new_pcb){
    printf("DISPATHCHER: SWITCH --> OLD:");
    (old_pcb != NULL) ? printf("%d", old_pcb->pid) : printf("NULL");
    printf(" - NEW:");
    (new_pcb != NULL) ? printf("%d",new_pcb->pid) : printf("NULL");
    printf("\n");
}

void disp_Terminate(pcb_t * pcb){
    printf("DISPATHCHER: Process with %d PID terminated", pcb->pid);
}

void sched_init(sched_basic_t * sched, clk_timer_s * timer, int SCHED_POLITIKA){ //TODO: Hay que configurar el inicilizador para que admita diferentes politicas
 
    // Schedulerraren mutex-a hasieratu
    sched->sched_mutex_list = (pthread_mutex_t * ) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(sched->sched_mutex_list, NULL);

    // Prozesu nulua
    pcb_init(&sched->null_process,-1,0);
    sched->current_running_process = &(sched->null_process);
    disp_Dispatch(NULL, sched->current_running_process);

    sched->current_politika = SCHED_POLITIKA;

    i_schedStartup(&(sched->scheduler_queue_iface_object), SCHED_POLITIKA);
    sched->timer = timer;
    sched->default_process_quantum = timer->tick_count;

    sched->current_sched_status = NULL_PROCESS_RUN;
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

