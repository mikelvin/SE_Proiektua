#include <stdlib.h>
#include <sheduler_s.h>
#include <pthread.h>
#include <linked_list_s.h>
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

    sched->current_running_process = next_process;
    disp_Dispatch(next_process, old_process);
    
    sched_AddToProcessQueue(sched, old_process);
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

