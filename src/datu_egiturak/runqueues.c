# include <runqueues_s.h>

void prio_array_init(rt_prio_array * pri_arr){
    pri_arr->nr_active = 0;
    for(int prio = 0 ; prio < MAX_PRIO; prio++){
        lnklst_LFRL_init(&pri_arr->queue[prio]);
    }
};

void runqueue_init(rt_runqueue * runq){
    prio_array_init(&runq->array[0]);
    prio_array_init(&runq->array[1]);
    runq->nr_running = 0;

    runq->active = &runq->array[0];
    runq->expired = &runq->array[1];

};  

void simple_runqueue_init(rt_simple_runqueue * runq){
    lnklst_LFRL_init(&runq->active);
    runq->nr_running = 0;
}
