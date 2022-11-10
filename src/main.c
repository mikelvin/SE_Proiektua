#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <linked_list_s.h>
#include <clk_s.h>
#include <pcb_s.h>
#include <sheduler_s.h>

#define TIMER_KOP   2 
#define SCHEDULER_FREQUENCY   1000 
#define PROCESSGEN_FREQUENCY   5000 

void prgen_generator(pr_gen_t * pr_gen);

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

// Scehduler


pthread_mutex_t nire_sched_list_mutex = PTHREAD_MUTEX_INITIALIZER;


int main()
{ 

    pthread_t tr_clk, tr_tmr1, tr_tmr2;

    sched_basic_t * my_scheduler;
    my_scheduler = (sched_basic_t *) malloc(sizeof(sched_basic_t));
    
    int timer_kop = TIMER_KOP;
    printf("Clock execute\n");
    if (pthread_create(&tr_clk, NULL, clock_start, (void *)  &clkm_1)){
        printf("Error: Clock started\n");
    }
    
    struct timer_s tmr1 = {.linked_clk = &clkm_1, .tmr_tick_freq = SCHEDULER_FREQUENCY, .TickAction=&sched_Schedule, .tickActionParams=my_scheduler};
    if (pthread_create(&tr_tmr1, NULL, timer_start, (void *) &tmr1)){
        printf("Error: Timer started\n"); 
    }

    struct timer_s tmr2 = {.linked_clk = &clkm_1, .tmr_tick_freq = PROCESSGEN_FREQUENCY, .TickAction=&prgen_generator, .tickActionParams=NULL};
    if (pthread_create(&tr_tmr2, NULL, timer_start, (void *) &tmr2)){
        printf("Error: Timer started\n"); 
    }
    
-    pthread_join(tr_clk, NULL);
    //pthread_join(tr_tmr1, NULL); 
    
    exit(0);
}


void prgen_generator(pr_gen_t * pr_gen){
    printf("Process generator called: ");

    pcb_t * new_process_pcb;
    new_process_pcb = (pcb_t *) malloc(sizeof(pcb_t));

    int new_pr_id;
    new_pr_id = rand() % (pr_gen->MAX_PID-pr_gen->MIN_PID) + pr_gen->MIN_PID;
    
    new_process_pcb->pid = new_pr_id;

}