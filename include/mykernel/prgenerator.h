#ifndef MY_PRGENERATOR_H
#define MY_PRGENERATOR_H

#include <scheduler_s.h>

struct pr_gener_egitura {
    int MAX_PID;
    int MIN_PID;
    int current_counter;
    int max_prio;
    sched_basic_t * scheduler;
};

typedef struct pr_gener_egitura pr_gen_t;

int _prgen_getValidPid(pr_gen_t * pr_gen);
void prgen_init(pr_gen_t *prgen, sched_basic_t * sched, int max_PID, int min_PID, int max_prio);
void prgen_generate(pr_gen_t * prgen);
void sched_print_sched_State(sched_basic_t * sched);

#endif