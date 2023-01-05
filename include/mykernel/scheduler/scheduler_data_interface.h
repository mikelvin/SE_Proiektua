#ifndef SCHED_DATA_IFACE
#define SCHED_DATA_IFACE

#include <pcb_s.h>
#include <string.h>

#define MY_SCHED_PRIO_RR 1
#define MY_SCHED_PRIO_FIFO 2
#define MY_SCHED_FIFO 3
#define MY_SCHED_RR 4


struct s_i_sched { // Sruct_Interface_Scheduler
    void * sched_data;
    void    (* insert)  (void * sched, pcb_t * new_pcb);
    void    (* init)    (void * sched);
    pcb_t *  (* peek)    (void * sched);
    pcb_t * (* out)     (void * sched);
    char * (* print)     (void * sched); //TODO: Make print Function
};
typedef  struct s_i_sched  s_i_sched;

void i_sched_init(s_i_sched * sched);
pcb_t * i_sched_peek(s_i_sched * sched);
pcb_t * i_sched_out(s_i_sched * sched);
void i_sched_in(s_i_sched * sched, pcb_t * new_pcb);
char * i_sched_print(s_i_sched * sched);

void i_schedImplement(
    s_i_sched * ifaceo,
    void * sched_data,
    void    (* insert)  (void * sched_data, pcb_t * new_pcb),
    void    (* init)    (void * sched_data),
    pcb_t *  (* peek)    (void * sched_data),
    pcb_t *  (* out)     (void * sched_data),
    char * (* print) (void * sched_data)
    );

void i_schedStartup(s_i_sched * ifaceo, int POLICY);

#endif
