#ifndef MY_PCB_H
#define MY_PCB_H


#define PR_WAIT 0
#define PR_EXEC 1
#define PR_FINI 2


struct pcb_str {
    int pid;
    unsigned int prio;
    unsigned int status;
};

typedef struct pcb_str pcb_t;

#endif
