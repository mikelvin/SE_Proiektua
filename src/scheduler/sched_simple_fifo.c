#include <schedp_simple_fifo.h>
#include <stdio.h>
#include <stdlib.h>

void rq_SIMPLE_FIFO_in(rt_simple_runqueue * sched, pcb_t * new_pcb){
    lnklst_LFRL_push(&sched->active, (void * ) new_pcb);
    sched->nr_running++;
};
void rq_SIMPLE_FIFO_init(rt_simple_runqueue * sched){
    simple_runqueue_init(sched);
};
pcb_t * rq_SIMPLE_FIFO_peek(rt_simple_runqueue * sched){
    if(sched->nr_running){
        return (pcb_t *) lnklst_LFRL_peek(&sched->active);
    }else{
        return NULL;
    }
};

pcb_t * rq_SIMPLE_FIFO_out(rt_simple_runqueue * sched){
    if(sched->nr_running){
        sched->nr_running--;
        return (pcb_t *) lnklst_LFRL_pop(&sched->active);
    }else{
        return NULL;
    }
}
char * rq_SIMPLE_FIFO_print(rt_simple_runqueue *sched)
{
    printf("Scheduler Info:\n");
    printf("- Process list len = %d \n", sched->active.len);
    printf("- Process pids = NEXT -> [ ");
    struct lnk_node * pr;
    for(pr = sched->active.first; pr != NULL;  pr = pr->next){
        pcb_t * current = (pcb_t *) pr->data;
        printf("%d ", current->pid);
    }
    printf("]\n");
    return NULL;
};

// s_i_sched INTERFAZEA IMPLEMENTATZEKO METODO SORTA:
// s_i_sched interfazea inplementatzen duen metodoa.
// Inplementazio hau RealTime queue bat erabiltzea ahalbidetzen du.
void rq_SIMPLE_FIFO_ISchedImplement(s_i_sched * isched){
    rt_simple_runqueue * sched_data = (rt_simple_runqueue*) malloc(sizeof(rt_simple_runqueue));
    
    i_schedImplement(
        isched,
        sched_data,
        &rq_SIMPLE_FIFO_ISchedAdpt_in,
        &rq_SIMPLE_FIFO_ISchedAdpt_init,
        &rq_SIMPLE_FIFO_ISchedAdpt_peek,
        &rq_SIMPLE_FIFO_ISchedAdpt_out,
        &rq_SIMPLE_FIFO_ISchedAdpt_print
        );
};
void rq_SIMPLE_FIFO_ISchedAdpt_in(void * sched_data, pcb_t * new_pcb){
    rq_SIMPLE_FIFO_in( (rt_simple_runqueue * ) sched_data, new_pcb);
};
void rq_SIMPLE_FIFO_ISchedAdpt_init(void * sched_data){
    rq_SIMPLE_FIFO_init((rt_simple_runqueue * ) sched_data);
};
pcb_t * rq_SIMPLE_FIFO_ISchedAdpt_peek(void * sched_data){
    return rq_SIMPLE_FIFO_peek((rt_simple_runqueue * ) sched_data);
};
pcb_t * rq_SIMPLE_FIFO_ISchedAdpt_out(void * sched_data){
    return rq_SIMPLE_FIFO_out((rt_simple_runqueue * ) sched_data);
};
char * rq_SIMPLE_FIFO_ISchedAdpt_print(void * sched_data){
    return rq_SIMPLE_FIFO_print((rt_simple_runqueue * ) sched_data);
};
