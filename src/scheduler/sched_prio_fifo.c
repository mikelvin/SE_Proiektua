
#include <schedp_prio_fifo.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>

int getFirstNotEmptyListIndex(lnklist_LFRL arr[]); // TODO: Move to runqueues_s.h

/*
    PCB-a RoundRobbin run queue-aren barnean gehitzen du. 
    Gehiketa hau expired datu egituran sartzen da, hurrengo zikloan exekutatzeko

    Funtzioa ez da konkurrentziarekiko SAFE.

*/
void rq_PRIO_FIFO_in(rt_runqueue * sched, pcb_t * new_pcb){
    int pcb_prio = new_pcb->curr_prio;
    rt_prio_array * prio_ar = sched->expired;
    // Frogatu txertatuko den PCB-a prioritate egokia duela
    if (MAX_PRIO <= pcb_prio){
        printf("\n\n Bad prio: %d \n", pcb_prio);
        err(1, "Priority max exceeded error. En rq_PRIO_RR_in(): El proceso tiene una prioridad mayor a la maxima.\n");
        
    }
    // Lortu dagokion prioritate ilara
    lnklist_LFRL * eq_prio_pr_list = &prio_ar->queue[pcb_prio];

    // Gehitu elementua ilarara
    lnklst_LFRL_push(eq_prio_pr_list, (void *) new_pcb);

    prio_ar->nr_active++;
    sched->nr_running++;
};
void rq_PRIO_FIFO_init(rt_runqueue * sched){
    runqueue_init(sched);
};
pcb_t * rq_PRIO_FIFO_peek(rt_runqueue * sched){
    _rq_PRIO_FIFO_updateSched(sched);
    rt_prio_array * current_prio_arr = sched->active;
    if(current_prio_arr->nr_active){
        int next_prio = getFirstNotEmptyListIndex(current_prio_arr->queue);
        return (pcb_t *) lnklst_LFRL_peek(&current_prio_arr->queue[next_prio]);
    }else{
        return NULL;
    }
};

pcb_t * rq_PRIO_FIFO_out(rt_runqueue * sched){
    _rq_PRIO_FIFO_updateSched(sched);
    rt_prio_array * current_prio_arr = sched->active;
    if(current_prio_arr->nr_active){
        current_prio_arr->nr_active--;
        sched->nr_running--;
        int next_prio = getFirstNotEmptyListIndex(current_prio_arr->queue);
        return (pcb_t *) lnklst_LFRL_pop(&current_prio_arr->queue[next_prio]);
    }else{
        return NULL;
    }
};

char * rq_PRIO_FIFO_print(rt_runqueue * sched){

    printf("- Process total list len = %d \n", sched->nr_running);
    printf("- Process active = %d ; expired = %d \n", sched->active->nr_active, sched->expired->nr_active);
    printf("Active: \n");
    printf("NEXT -> ");
    for (int i = 0; i < MAX_PRIO; i++)
    {
        if(sched->active->queue[i].len != 0){
            printf("[%d] -> [",i);
            struct lnk_node * pr;
            for(pr = sched->active->queue[i].first; pr != NULL;  pr = pr->next){
                pcb_t * current = (pcb_t *) pr->data;
                printf("%d ", current->pid);
            }
            printf("]\n        ");
        }
    }
    printf("\n");
    printf("EXPIRED:\n   ");
    for (int i = 0; i < MAX_PRIO; i++)
    {
        if(sched->expired->queue[i].len != 0){
            printf("[%d] -> [",i);
            struct lnk_node * pr;
            for(pr = sched->expired->queue[i].first; pr != NULL;  pr = pr->next){
                pcb_t * current = (pcb_t *) pr->data;
                printf("%d ", current->pid);
            }
            printf("]\n   ");
        }
    }
    return NULL;
}

int _rq_PRIO_FIFO_updateSched(rt_runqueue * sched){
    if(sched->active->nr_active > 0 || // Active notEmpty edo
        sched->expired->nr_active <= 0){ // Expired Empty
        // Biak hutsik, Active hutsik eta Expired (EZ edo BAI) hutsik
        return 0;
    }
    // Expired prozesuak baditu eta activek-ez, orduan: Expired eta active trukatu
    rt_prio_array * current_prio_arr = sched->active;
    sched->active = sched->expired;
    sched->expired = current_prio_arr;
    return 1;
        
}

int getFirstNotEmptyListIndex(lnklist_LFRL arr[]){ // TODO: Move to runqueues_s.h
    int current_index = 0;
    while (current_index < MAX_PRIO){
        if(arr[current_index].len){
            return current_index;
        }
        current_index++;
    }
    return -1;
}

// s_i_sched INTERFAZEA IMPLEMENTATZEKO METODO SORTA:
// s_i_sched interfazea inplementatzen duen metodoa.
// Inplementazio hau RealTime Priority queue bat erabiltzea ahalbidetzen du.
void rq_PRIO_FIFO_ISchedImplement(s_i_sched * isched){
    rt_runqueue * sched_data = (rt_runqueue*) malloc(sizeof(rt_runqueue));
    
    i_schedImplement(
        isched,
        sched_data,
        &rq_PRIO_FIFO_ISchedAdpt_in,
        &rq_PRIO_FIFO_ISchedAdpt_init,
        &rq_PRIO_FIFO_ISchedAdpt_peek,
        &rq_PRIO_FIFO_ISchedAdpt_out,
        &rq_PRIO_FIFO_ISchedAdpt_print
        );
};
void rq_PRIO_FIFO_ISchedAdpt_in(void * sched_data, pcb_t * new_pcb){
    rq_PRIO_FIFO_in( (rt_runqueue * ) sched_data, new_pcb);
};
void rq_PRIO_FIFO_ISchedAdpt_init(void * sched_data){
    rq_PRIO_FIFO_init((rt_runqueue * ) sched_data);
};
pcb_t * rq_PRIO_FIFO_ISchedAdpt_peek(void * sched_data){
    return rq_PRIO_FIFO_peek((rt_runqueue * ) sched_data);
};
pcb_t * rq_PRIO_FIFO_ISchedAdpt_out(void * sched_data){
    return rq_PRIO_FIFO_out((rt_runqueue * ) sched_data);
};
char * rq_PRIO_FIFO_ISchedAdpt_print(void * sched_data){
    return rq_PRIO_FIFO_print((rt_runqueue * ) sched_data);
};