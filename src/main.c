#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <lnklist_s.h>
#include <clk_s.h>
#include <pcb_s.h>
#include <sheduler_s.h>
#include <cpu_data_s.h>

#define TIMER_KOP   3 
#define SCHEDULER_FREQUENCY   10 
#define PROCESSGEN_FREQUENCY   5 
#define EGOERA_PRINT_FREQUENCY 5

#define PRGEN_MAX_PID 1
#define PRGEN_MIN_PID 1000


pthread_mutex_t g_mutex_clk = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_cond_clk_ziklo = PTHREAD_COND_INITIALIZER;
pthread_cond_t g_cond_tmr_arreta = PTHREAD_COND_INITIALIZER;

struct clk_mutex_s clkm_1 = {
        .mutex_clk = &g_mutex_clk,
        .cond_clk_ziklo = &g_cond_clk_ziklo,
        .cond_tmr_arreta = &g_cond_tmr_arreta,
        .tmr_kop_tick_consumed = 0,
        .tmr_total_kop = TIMER_KOP
    };


int main()
{ 

    pthread_t tr_clk, tr_tmr1, tr_tmr2_prgen, tr_tmr_print;

    sched_basic_t * my_scheduler;
    my_scheduler = sched_struct_create_and_init();

    pr_gen_t my_pr_generator;
    prgen_init(&my_pr_generator, my_scheduler, PRGEN_MAX_PID, PRGEN_MIN_PID);

    int timer_kop = TIMER_KOP;
    printf("Clock execute\n");
    if (pthread_create(&tr_clk, NULL, clock_start, (void *)  &clkm_1)){
        printf("Error: Clock started\n");
    }
    
    struct timer_s tmr1 = {
        .linked_clk = &clkm_1, 
        .tmr_tick_freq = SCHEDULER_FREQUENCY, 
        .TickAction=&sched_Schedule, 
        .tickActionParams=my_scheduler};
    if (pthread_create(&tr_tmr1, NULL, timer_start, (void *) &tmr1)){
        printf("Error: Timer started\n"); 
    }

    struct timer_s tmr2_prgen = {.linked_clk = &clkm_1, .tmr_tick_freq = PROCESSGEN_FREQUENCY, .TickAction=&prgen_generate, .tickActionParams=&my_pr_generator};
    if (pthread_create(&tr_tmr2_prgen, NULL, timer_start, (void *) &tmr2_prgen)){
        printf("Error: Timer started\n"); 
    }

    struct timer_s tmr3 = {.linked_clk = &clkm_1, .tmr_tick_freq = EGOERA_PRINT_FREQUENCY, .TickAction=&sched_print_sched_State, .tickActionParams=my_scheduler};
    if (pthread_create(&tr_tmr_print, NULL, timer_start, (void *) &tmr3)){
        printf("Error: Timer started\n"); 
    }
    
-    pthread_join(tr_clk, NULL);
    //pthread_join(tr_tmr1, NULL); 
    
    exit(0);
}


