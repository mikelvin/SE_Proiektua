#ifndef MY_SHEDP_PRIO_FIFO
#define MY_SHEDP_PRIO_FIFO

#include <scheduler_s.h>
#include <runqueues_s.h>

// MY_SCHED_PRIO_FIFO
void rq_PRIO_FIFO_in(rt_runqueue * sched, pcb_t * new_pcb);
void rq_PRIO_FIFO_init(rt_runqueue * sched);
pcb_t * rq_PRIO_FIFO_peek(rt_runqueue * sched);
pcb_t * rq_PRIO_FIFO_out(rt_runqueue * sched);
char * rq_PRIO_FIFO_print(rt_runqueue * sched);
int _rq_PRIO_FIFO_updateSched(rt_runqueue * sched);

void rq_PRIO_FIFO_ISchedImplement(s_i_sched * isched);

void rq_PRIO_FIFO_ISchedAdpt_in(void * sched_data, pcb_t * new_pcb);
void rq_PRIO_FIFO_ISchedAdpt_init(void * sched_data);
pcb_t * rq_PRIO_FIFO_ISchedAdpt_peek(void * sched_data);
pcb_t * rq_PRIO_FIFO_ISchedAdpt_out(void * sched_data);
char * rq_PRIO_FIFO_ISchedAdpt_print(void * sched_data);
#endif