#include <pcb_s.h>

void pcb_init(pcb_t * pcb, int pid, int current_prio){
    pcb->pid = pid;
    pcb->curr_prio = current_prio;
    pcb->status = PR_UNDEFINED;
};

int pcb_getStatus(pcb_t * pcb){
    return pcb->curr_prio;
};