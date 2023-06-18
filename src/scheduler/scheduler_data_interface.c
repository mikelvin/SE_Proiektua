#include <scheduler_data_interface.h>
#include <schedp_simple_fifo.h>
#include <schedp_prio_fifo.h>

// Sheduler Interface:
void i_schedImplement(
    s_i_sched * ifaceo,
    void * sched_data,
    void    (* insert)  (void * sched_data, pcb_t * new_pcb),
    void    (* init)    (void * sched_data),
    pcb_t  * (* peek)    (void * sched_data),
    pcb_t  * (* out)     (void * sched_data),
    char * (* print) (void * sched_data)
    ){
        ifaceo->sched_data = sched_data;
        ifaceo->init = init;
        ifaceo->peek = peek;
        ifaceo->out = out;
        ifaceo->insert = insert;
        ifaceo->print = print;
    }

void i_sched_init(s_i_sched * sched){
    sched->init(sched->sched_data);
};
pcb_t * i_sched_peek(s_i_sched * sched){
    return sched->peek(sched->sched_data);
};
pcb_t * i_sched_out(s_i_sched * sched){
    return sched->out(sched->sched_data);
};
void i_sched_in(s_i_sched * sched, pcb_t * new_pcb){
    sched->insert(sched->sched_data, new_pcb);
};
char * i_sched_print(s_i_sched * sched){
    sched->print(sched->sched_data);
};

void  i_schedConfigure(s_i_sched * ifaceo, int POLICY){
    switch (POLICY)
    {
    case MY_SCHED_PRIO_RR:
    case MY_SCHED_PRIO_FIFO:
        rq_PRIO_FIFO_ISchedImplement(ifaceo);
        break;

    case MY_SCHED_FIFO:
    case MY_SCHED_RR:    
    default:
        rq_SIMPLE_FIFO_ISchedImplement(ifaceo);
        break;
    }
    i_sched_init(ifaceo);
};