#ifndef CLOCK_H
#define CLOCK_H

# include <pthread.h>
typedef void (*DoActionWhenTick)();
struct timer_s{
    struct clk_mutex_s * linked_clk;
    int tmr_tick_freq;
    DoActionWhenTick TickAction;
    void * tickActionParams;
};

struct clk_mutex_s{
    pthread_mutex_t * mutex_clk;
    pthread_cond_t * cond_clk_ziklo;
    pthread_cond_t * cond_tmr_arreta;
    int tmr_kop_tick_consumed;
    int tmr_total_kop;
};

void *timer_start(void *tmr_struct);
void *clock_start(void *clk_struct);

#endif