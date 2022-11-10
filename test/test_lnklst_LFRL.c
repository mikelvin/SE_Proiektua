#include <stdio.h>
#include <linked_list_s.h>
#include <pcb_s.h>


int main(){
    struct pcb_t m_pcb1 = {.pid = 10};
    struct pcb_t m_pcb2 = {.pid = 11};

    struct pcb_t * m_pcb2_pntr1, * m_pcb2_pntr2;
    

    lnklist_LFRL m_list;
    lnklst_LFRL_init(&m_list);

    lnklst_LFRL_push(&m_list, &m_pcb1);
    //pr_lst_LFRL_push(&m_list, &m_pcb2);

    m_pcb2_pntr1 = lnklst_LFRL_pop(&m_list);
    m_pcb2_pntr2 = lnklst_LFRL_pop(&m_list);

    printf("PID: %d \n", m_pcb2_pntr1->pid );
    printf("PID: %d \n", m_pcb2_pntr2->pid );
}