#ifndef bitmp
#define CLOCK_H

#include <lnklist_s.h>
#include <bitmap.h>
#include <lnklist_s.h>

#define MAX_PRIO 10

struct prio_array
{
    int nr_active;
    struct bit_map bitmap;
    struct list_head queue[MAX_PRIO];
};

struct  runqueue
{
    int nr_running;
    struct rt_prio_array *active;
    struct rt_prio_array *expired;
    struct rt_prio_array arrays[2];
};

struct prio_array prio_array_init();

struct runqueue runqueue_init();

