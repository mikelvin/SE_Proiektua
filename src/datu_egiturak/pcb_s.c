#include <pcb_s.h>

void pcb_init(pcb_t * pcb, int pid, int current_prio){
    pcb->pid = pid;
    pcb->curr_prio = current_prio;
    pcb->status = PR_UNDEFINED;
};

int pcb_getStatus(pcb_t * pcb){
    return pcb->curr_prio;
};

int mm_init(struct mm * p_mm, uint32_t pgb, uint32_t code_virt_adress, uint32_t data_virt_adress){
    p_mm->pgb = pgb;
    p_mm->code = code_virt_adress;
    p_mm->data = data_virt_adress;
}