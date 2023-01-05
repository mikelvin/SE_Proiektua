#ifndef MY_SCHEDULER_H
#define MY_SCHEDULER_H

#include <pthread.h>
#include <lnklist_s.h>
#include <pcb_s.h>
#include <runqueues_s.h>

#include <clk_s.h>

#include <scheduler_data_interface.h>

# define NULL_PROCESS_RUN 1
# define PROCESS_RUN 2


typedef struct sched_egitura sched_basic_t;

struct sched_egitura {
    pthread_mutex_t * sched_mutex_list;

    int current_politika; 
    s_i_sched scheduler_queue_iface_object;

    pcb_t * current_running_process;

    int current_sched_status;

    pcb_t null_process;

    // FOR: Quantum bidezko kanporaketa
    clk_timer_s * timer;
    int default_process_quantum;
    
};

void sched_AddToProcessQueue(sched_basic_t * sched, pcb_t * new_pcb);
void sched_Scheduler_Dispatcher(sched_basic_t * sched);
pcb_t * sched_GetNextFromProcessQueue(sched_basic_t * sched);
void disp_Dispatch(pcb_t * old_pcb, pcb_t * new_pcb);
void sched_init(sched_basic_t * sched, clk_timer_s * timer, int SCHED_POLITIKA);

#endif
