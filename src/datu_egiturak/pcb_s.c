#include <pcb_s.h>

void pcb_init(pcb_t * pcb, int pid, int current_prio){
    pcb->pid = pid;
    pcb->curr_prio = current_prio;
    pcb->status = PR_UNDEFINED;
}
void pcb_destroy(pcb_t *pcb){
    free(pcb);
};

int pcb_getStatus(pcb_t * pcb){
    return pcb->curr_prio;
};

int mm_init(struct mm * p_mm, uint32_t pgb, uint32_t code_virt_adress, uint32_t data_virt_adress){
    p_mm->pgb = pgb;
    p_mm->code = code_virt_adress;
    p_mm->data = data_virt_adress;
}

void cpu_snapshot_init(struct cpu_snapshot * cs, uint32_t PC){
    cs->cc = 0;
    cs->IR = 0;
    cs->PC = PC;
    for(int i = 0; i < CPU_HARI_REG_KOP; i++){
        cs->R[i]=0;
    }
}