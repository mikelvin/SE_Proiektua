#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <lnklist_s.h>
#include <clk_s.h>

void *clk_clock_start(void *clk_mutex_s){
    clk_clock_s * clk;
    clk = (clk_clock_s *)clk_mutex_s;

    pthread_mutex_lock(clk->mutex_clk); // Mutexa blokeatu
    while(1){
        printf("\n\n+ CLK: tick %d - %d \n", clk->tmr_kop_tick_consumed, clk->tmr_total_kop);
        while (clk->tmr_kop_tick_consumed < clk->tmr_total_kop || 
           clk->tmr_total_kop == 0)
        {
            pthread_cond_wait(clk->cond_tmr_arreta, clk->mutex_clk );
        }
        pthread_cond_broadcast(clk->cond_clk_ziklo);
        clk->tmr_kop_tick_consumed = 0;
        sleep(1);
    }
    pthread_mutex_unlock(clk->mutex_clk);
}

void *clk_timer_start(void *p_timer_s){
    clk_timer_s * tmr;
    clk_clock_s * t_clk;
    
    tmr = (clk_timer_s *) p_timer_s;
    t_clk = tmr->linked_clk;
    tmr->tick_count = tmr->tmr_tick_freq;

    pthread_mutex_lock(t_clk->mutex_clk);
    t_clk->tmr_total_kop++;

    while(tmr->tmr_status != CLK_STATUS_TERMINATE){
        tmr->tick_count -= 1;
        printf("TMR_f%d: count %d \n",tmr->tmr_tick_freq, tmr->tick_count);
        
        if (tmr->tick_count <= 0){
            tmr->TickAction(tmr->tickActionParams);
            tmr->tick_count = tmr->tmr_tick_freq;
        }
        t_clk->tmr_kop_tick_consumed++;
        pthread_cond_signal(t_clk->cond_tmr_arreta);
        pthread_cond_wait(t_clk->cond_clk_ziklo, t_clk->mutex_clk);
    }

    t_clk->tmr_total_kop--;
    pthread_mutex_unlock(t_clk->mutex_clk);
}

void clk_clock_init(clk_clock_s * clk_struct){
    pthread_mutex_t * g_mutex_clk = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    pthread_cond_t * g_cond_clk_ziklo = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));
    pthread_cond_t * g_cond_tmr_arreta = (pthread_cond_t *) malloc(sizeof(pthread_cond_t));

    pthread_mutex_init(g_mutex_clk, NULL);
    pthread_cond_init(g_cond_clk_ziklo, NULL);
    pthread_cond_init(g_cond_tmr_arreta, NULL);
    
    clk_struct->mutex_clk = g_mutex_clk;
    clk_struct->cond_clk_ziklo = g_cond_clk_ziklo;
    clk_struct->cond_tmr_arreta = g_cond_tmr_arreta;
    clk_struct->tmr_kop_tick_consumed = 0;
    clk_struct->tmr_total_kop = 0;
    clk_struct->clk_status = CLK_STATUS_RUNNING;
}

void clk_timer_init(clk_timer_s *tmr_struct, clk_clock_s *clk_struct, int tmr_tick_freq, DoActionWhenTick tickaction, void * tickActionParams){
    tmr_struct->linked_clk = clk_struct;
    tmr_struct->tmr_tick_freq = tmr_tick_freq;
    tmr_struct->TickAction = tickaction;
    tmr_struct->tickActionParams = tickActionParams;
    tmr_struct->tmr_status = CLK_STATUS_RUNNING;
    tmr_struct->tick_count = 0;
}

void clk_stop_timer(clk_timer_s *tmr_struct){
    tmr_struct->tmr_status = CLK_STATUS_TERMINATE;
}

void clk_change_timer_tickFreq(clk_timer_s * tmr_struct, int new_tickFreq, int restart){
    if(restart){
        tmr_struct->tick_count = new_tickFreq;
    }else{
        int consumed_ticks = tmr_struct->tmr_tick_freq - tmr_struct->tick_count;
        tmr_struct->tick_count = new_tickFreq - consumed_ticks;
        if(tmr_struct->tick_count < 1){
            tmr_struct->tick_count = 1;
        }
    }
    tmr_struct->tmr_tick_freq = new_tickFreq;
}
