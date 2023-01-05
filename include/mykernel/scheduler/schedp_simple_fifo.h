#ifndef MY_SHEDP_SIMPLE_FIFO
#define MY_SHEDP_SIMPLE_FIFO

#include <scheduler_s.h>
#include <runqueues_s.h>

// SCHED_SIMPLE_FIFO
void rq_SIMPLE_FIFO_in(rt_simple_runqueue * sched, pcb_t * new_pcb);
void rq_SIMPLE_FIFO_init(rt_simple_runqueue * sched);
pcb_t * rq_SIMPLE_FIFO_peek(rt_simple_runqueue * sched);
pcb_t * rq_SIMPLE_FIFO_out(rt_simple_runqueue * sched);
char * rq_SIMPLE_FIFO_print(rt_simple_runqueue * sched);

void rq_SIMPLE_FIFO_ISchedImplement(s_i_sched * isched);

void rq_SIMPLE_FIFO_ISchedAdpt_in(void * sched, pcb_t * new_pcb);
void rq_SIMPLE_FIFO_ISchedAdpt_init(void * sched_data);
pcb_t * rq_SIMPLE_FIFO_ISchedAdpt_peek(void * sched_data);
pcb_t * rq_SIMPLE_FIFO_ISchedAdpt_out(void * sched_data);
char * rq_SIMPLE_FIFO_ISchedAdpt_print(void * sched_data);
#endif