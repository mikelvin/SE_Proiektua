#ifndef CLOCK_H
#define CLOCK_H

# include <pthread.h>

#define CLK_STATUS_RUNNING 1
#define CLK_STATUS_TERMINATE 2


typedef void (*DoActionWhenTick)();
struct clk_timer_s{
    struct clk_clock_s * linked_clk;
    int tmr_tick_freq;
    DoActionWhenTick TickAction;
    void * tickActionParams;
    int tmr_status;
    int tick_count;
};
typedef struct clk_timer_s clk_timer_s;

struct clk_clock_s{
    pthread_mutex_t * mutex_clk;
    pthread_cond_t * cond_clk_ziklo;
    pthread_cond_t * cond_tmr_arreta;
    int tmr_kop_tick_consumed;
    int tmr_total_kop;
    int clk_status;
};
typedef struct clk_clock_s clk_clock_s;

void *clk_timer_start(void *tmr_struct);
void *clk_clock_start(void *clk_struct);

void clk_clock_init(clk_clock_s *clk_struct);

void clk_timer_init(clk_timer_s *tmr_struct, clk_clock_s *clk_struct, 
    int tmr_tick_freq, DoActionWhenTick tickaction, void * tickActionParams); // TODO:

void clk_stop_timer(clk_timer_s *tmr_struct); // TODO:


void clk_change_timer_tickFreq(clk_timer_s * tmr_struct, int new_tickFreq, int restart); // TODO:


#endif