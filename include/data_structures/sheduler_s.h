#ifndef MY_SCHEDULER_H
#define MY_SCHEDULER_H

#include <pthread.h>
#include <linked_list_s.h>
#include <pcb_s.h>

struct sched_egitura {
    lnklist_LFRL * pr_l;
    pthread_mutex_t * sched_list_mutex;
    pcb_t * current_running_process;
};

typedef struct sched_egitura sched_basic_t;

struct pr_gener_egitura {
    int MAX_PID;
    int MIN_PID;
    int current_counter;
    sched_basic_t * scheduler;
};

typedef struct pr_gener_egitura pr_gen_t;

void sched_AddToProcessQueue(sched_basic_t * sched, pcb_t * new_pcb);
void sched_Schedule(sched_basic_t * sched);
pcb_t * sched_GetNextFromProcessQueue(sched_basic_t * sched);
void disp_Dispatch(pcb_t * old_pcb, pcb_t * new_pcb);

#endif
