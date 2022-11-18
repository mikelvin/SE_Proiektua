#ifndef MY_SCHEDULER_H
#define MY_SCHEDULER_H

#include <pthread.h>
#include <lnklist_s.h>
#include <pcb_s.h>
#include <runqueues_s.h>

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
void sched_Scheduler_Dispatcher(sched_basic_t * sched);
pcb_t * sched_GetNextFromProcessQueue(sched_basic_t * sched);
void disp_Dispatch(pcb_t * old_pcb, pcb_t * new_pcb);
sched_basic_t * sched_struct_create_and_init();

int _prgen_getValidPid(pr_gen_t * pr_gen);
void prgen_init(pr_gen_t *prgen, sched_basic_t * sched, int max_PID, int min_PID);
void prgen_generate(pr_gen_t * prgen);
void sched_print_sched_State(sched_basic_t * sched);
#endif
