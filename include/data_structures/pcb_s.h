#ifndef MY_PCB_H
#define MY_PCB_H

#define PR_UNDEFINED 0
#define PR_EXEC 1
#define PR_FINI 2

# include <stdint.h>

struct mm {
    uint32_t data; // Virtual data adress
    uint32_t code; // Virtual code adress
    uint32_t pgb;  // Orri taularen helbide fisikoa
};

struct pcb_str {
    int pid;
    int curr_prio;
    int status;
    struct mm memo_m; // Memory management
};

typedef struct pcb_str pcb_t;

int mm_init(struct mm * p_mm, uint32_t pgb, uint32_t code_virt_adress, uint32_t data_virt_adress);

void pcb_init(pcb_t * pcb, int pid, int current_prio);

int pcb_getStatus(pcb_t * pcb);

#endif 
