#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <lnklist_s.h>
#include <clk_s.h>
#include <pcb_s.h>
#include <scheduler_s.h>
#include <cpu_data_s.h>
#include <prgenerator.h>
#include <schedp_prio_fifo.h>

#define TIMER_KOP   3 
#define SCHEDULER_FREQUENCY  2
#define PROCESSGEN_FREQUENCY   4 
#define EGOERA_PRINT_FREQUENCY 1

#define PRGEN_MAX_PID 1
#define PRGEN_MIN_PID 1000


struct clk_clock_s clkm_1;

int main()
{ 
    clk_timer_s tmr1_scheduler;
    clk_timer_s tmr2_prgen;
    clk_timer_s debug_tmr3_print_pqueue;
    pthread_t tr_clk, tr_tmr1_sched, tr_tmr2_prgen, tr_tmr_print;

    pr_gen_t my_pr_generator;
    sched_basic_t my_scheduler;

    clk_clock_init(&clkm_1);
    
    clk_timer_init(&tmr1_scheduler,&clkm_1, 
        SCHEDULER_FREQUENCY, sched_Scheduler_Dispatcher, &my_scheduler);
    clk_timer_init(&tmr2_prgen,&clkm_1, 
        PROCESSGEN_FREQUENCY,&prgen_generate,&my_pr_generator);
    clk_timer_init(&debug_tmr3_print_pqueue, &clkm_1, 
        EGOERA_PRINT_FREQUENCY, &sched_print_sched_State, &my_scheduler);

    
    
    sched_init(&my_scheduler, &tmr1_scheduler, MY_SCHED_PRIO_RR);
    prgen_init(&my_pr_generator, &my_scheduler, PRGEN_MAX_PID, PRGEN_MIN_PID, MAX_PRIO);

      
    if (pthread_create(&tr_tmr1_sched, NULL, clk_timer_start, (void *) &tmr1_scheduler))
        printf("Error: Timer started\n"); 
    
    if (pthread_create(&tr_tmr2_prgen, NULL, clk_timer_start, (void *) &tmr2_prgen))
        printf("Error: Timer started\n"); 
    
    if (pthread_create(&tr_tmr_print, NULL, clk_timer_start, (void *) &debug_tmr3_print_pqueue))
        printf("Error: Timer started\n"); 
    
    printf("Clock execute\n");
    if (pthread_create(&tr_clk, NULL, clk_clock_start, (void *)  &clkm_1))
        printf("Error: Clock started\n");
    

-   pthread_join(tr_clk, NULL);
     
    
    exit(0);
}


