#ifndef MY_RUNQUEUES_H
#define MY_RUNQUEUES_H

#include <lnklist_s.h>

#define MAX_PRIO 10

struct rt_prio_array
{
    int nr_active;
    lnklist_LFRL queue[MAX_PRIO];
};

typedef struct rt_prio_array rt_prio_array;

struct  rt_runqueue
{
    int nr_running;
    rt_prio_array *active;
    rt_prio_array *expired;
    rt_prio_array array [2];
};
typedef struct rt_runqueue rt_runqueue;

struct  rt_simple_runqueue
{
    int nr_running;
    lnklist_LFRL active;
};
typedef struct rt_simple_runqueue rt_simple_runqueue;

void prio_array_init(rt_prio_array * pri_arr);

void runqueue_init(rt_runqueue * runq);

void simple_runqueue_init(rt_simple_runqueue * runq);

// Get list giving a priority
lnklist_LFRL * rt_priarr_getList(rt_prio_array rt_priarr, int prio);

int rt_priarr_getListLength(rt_prio_array rt_priarr, int prio);

#endif 
