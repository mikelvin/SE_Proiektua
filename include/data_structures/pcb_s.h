#ifndef MY_PCB_H
#define MY_PCB_H

#define PR_UNDEFINED 0
#define PR_WAIT 1
#define PR_EXEC 2
#define PR_FINI 3


struct pcb_str {
    int pid;
    int curr_prio;
    int status;
};

typedef struct pcb_str pcb_t;

void pcb_init(pcb_t * pcb, int pid, int current_prio);

int pcb_getStatus(pcb_t * pcb);

#endif 
