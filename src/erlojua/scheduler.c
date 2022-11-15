#include <stdlib.h>
#include <stdio.h>
#include <sheduler_s.h>
#include <pthread.h>
#include <lnklist_s.h>
#include <pcb_s.h>

void sched_AddToProcessQueue(sched_basic_t * sched, pcb_t * new_pcb){
    pthread_mutex_lock(sched->sched_list_mutex);
    lnklst_LFRL_push(sched->pr_l, (void * ) new_pcb);
    pthread_mutex_unlock(sched->sched_list_mutex);
}

void sched_Schedule(sched_basic_t * sched){
    pcb_t * next_process, * old_process;
    next_process = sched_GetNextFromProcessQueue(sched);
    old_process = sched->current_running_process;

    if (next_process != NULL){
        sched->current_running_process = next_process;
        disp_Dispatch(next_process, old_process);

        sched_AddToProcessQueue(sched, old_process);
    }
}

pcb_t * sched_GetNextFromProcessQueue(sched_basic_t * sched){
    pcb_t * next_process;
    pthread_mutex_lock(sched->sched_list_mutex);
    next_process = (pcb_t *) lnklst_LFRL_pop(sched->pr_l);
    pthread_mutex_unlock(sched->sched_list_mutex);
    return next_process;
}

void disp_Dispatch(pcb_t * old_pcb, pcb_t * new_pcb){
    
}

sched_basic_t * sched_struct_create_and_init(){

    // Sortu sheduler struct-a memorian
    sched_basic_t * nire_sched;
    nire_sched = (sched_basic_t * ) malloc(sizeof(sched_basic_t));
    
    // Hasieratu pr_list-a: Memoria alokatu + hasieratu
    nire_sched->pr_l = (lnklist_LFRL * ) malloc(sizeof(lnklist_LFRL));
    lnklst_LFRL_init(nire_sched->pr_l);

    // Schedulerraren mutex-a hasieratu
    nire_sched->sched_list_mutex = (pthread_mutex_t * ) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(nire_sched->sched_list_mutex, NULL);

    // Prozesu nulua
    pcb_t * null_process = (pcb_t *) malloc(sizeof(pcb_t));
    nire_sched->current_running_process = null_process; 
    return nire_sched;
}

void prgen_init(pr_gen_t *prgen, sched_basic_t * sched, int max_PID, int min_PID){
    prgen->MAX_PID = max_PID;
    prgen->MIN_PID = min_PID;
    prgen->scheduler = sched;
    prgen->current_counter = 0;
}

int _prgen_getValidPid(pr_gen_t * pr_gen){
    //TODO: Frogatu scheduler-ean pid hori jada esleituta dagoen ala ez
    int new_pr_id;
    new_pr_id = rand() % (pr_gen->MAX_PID-pr_gen->MIN_PID) + pr_gen->MIN_PID;
    return new_pr_id;
}

void prgen_generate(pr_gen_t * pr_gen){
    printf("Process generator called: ");

    pcb_t * new_process_pcb;
    new_process_pcb = (pcb_t *) malloc(sizeof(pcb_t));

    new_process_pcb->pid = _prgen_getValidPid(pr_gen);

    sched_AddToProcessQueue(pr_gen->scheduler, new_process_pcb);
}

void sched_print_sched_State(sched_basic_t * sched){
    pthread_mutex_lock(sched->sched_list_mutex);

    printf("Scheduler Info:\n");
    printf("- Process list len = %d \n", sched->pr_l->len);
    printf("- Process pids = NEXT -> [ ");
    struct lnk_node * pr;
    for(pr = sched->pr_l->first; pr != NULL;  pr = pr->next){
        pcb_t * current = (pcb_t *) pr->data;
        printf("%d ", current->pid);
    }
    printf("]\n");

    printf("- Current process = %d \n", sched->current_running_process->pid);

    pthread_mutex_unlock(sched->sched_list_mutex);
}

