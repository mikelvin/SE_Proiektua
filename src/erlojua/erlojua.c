#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <lnklist_s.h>
#include <clk_s.h>

void *clock_start(void *clk_mutex_s){
    struct clk_mutex_s * clk = (struct clk_mutex_s *)clk_mutex_s;

    pthread_mutex_lock(clk->mutex_clk); // Mutexa blokeatu
    while(1){
        clk->tmr_kop_tick_consumed = 0;   
        printf("CLK: tick\n");
        while ( clk->tmr_kop_tick_consumed < clk->tmr_total_kop)
        {
            pthread_cond_wait(clk->cond_tmr_arreta, clk->mutex_clk );
        }
        pthread_cond_broadcast(clk->cond_clk_ziklo);
    }
}

void *timer_start(void *p_timer_s){
    struct timer_s * tmr = (struct timer_s *) p_timer_s;

    struct clk_mutex_s * t_clk = tmr->linked_clk;

    int count = tmr->tmr_tick_freq;

    pthread_mutex_lock(t_clk->mutex_clk);
    
    while(1){
        count -= 1;
        printf("TMR_f%d: count %d \n",tmr->tmr_tick_freq, count);
        
        if (count <= 0){
            tmr->TickAction(tmr->tickActionParams);
            count = tmr->tmr_tick_freq;
        }
        t_clk->tmr_kop_tick_consumed++;
        pthread_cond_signal(t_clk->cond_tmr_arreta);
        pthread_cond_wait(t_clk->cond_clk_ziklo, t_clk->mutex_clk);
    }
}