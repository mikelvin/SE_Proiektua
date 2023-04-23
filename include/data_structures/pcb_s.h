#ifndef MY_PCB_H
#define MY_PCB_H

#define PR_UNDEFINED 0
#define PR_EXEC 1
#define PR_READY 3
#define PR_FINI 2

# include <stdint.h>
# include <executor.h>

struct mm {
    uint32_t data; // Virtual data adress
    uint32_t code; // Virtual code adress
    uint32_t pgb;  // Orri taularen helbide fisikoa
};

struct cpu_snapshot
{
    uint32_t PC; // Program Conter Erregistroa 
    uint32_t IR; //
    int32_t R[CPU_HARI_REG_KOP];
    int32_t cc;
};


struct pcb_str {
    int pid;
    int curr_prio;
    int status;
    struct mm memo_m; // Memory management
    struct cpu_snapshot s;
};

typedef struct pcb_str pcb_t;

int mm_init(struct mm * p_mm, uint32_t pgb, uint32_t code_virt_adress, uint32_t data_virt_adress);

void pcb_init(pcb_t * pcb, int pid, int current_prio);
void pcb_destroy(pcb_t * pcb);

void cpu_snapshot_init(struct cpu_snapshot * cs, uint32_t PC);

int pcb_getStatus(pcb_t * pcb);

#endif 
