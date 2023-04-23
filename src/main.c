#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <lnklist_s.h>
#include <clk_s.h>
#include <pcb_s.h>
#include <memory_s.h>
#include <scheduler_s.h>
#include <cpu_data_s.h>
#include <prgenerator.h>
#include <schedp_prio_fifo.h>
#include <loader.h>
#include <executor.h>

#define TIMER_KOP   3 
#define SCHEDULER_FREQUENCY  2
#define PROCESSGEN_FREQUENCY   4 
#define EGOERA_PRINT_FREQUENCY 1

#define PRGEN_MAX_PID 1
#define PRGEN_MIN_PID 1000

#define CPU_KOP 1
#define CORExCPU_KOP 1
#define HARIxCORE_KOP 1
#define CORE_TOTAL CPU_KOP*CORExCPU_KOP
#define HARI_TOTAL CORE_TOTAL*HARIxCORE_KOP


struct clk_clock_s clkm_1;

int main()
{ 
    clk_timer_s tmr1_scheduler;
    clk_timer_s tmr2_prgen;
    clk_timer_s debug_tmr3_print_pqueue;
    clk_timer_s tmr3_executor;
    pthread_t tr_clk, tr_tmr1_sched, tr_tmr2_prgen, tr_tmr_print, tr_tmr3_exec;

    pr_gen_t my_pr_generator;
    loader_s my_loader;
    sched_basic_t my_scheduler;
    struct physycal_memory system_memory;
    

    loader_startup(&my_loader, &system_memory, &my_scheduler, MAX_PRIO);

    pmemo_init(&system_memory);
    struct cpu_s my_cpu_arr [CPU_KOP];
    for(int i = 0;i < CPU_KOP; i++) cpu_t_init(&my_cpu_arr[i], CORExCPU_KOP, HARIxCORE_KOP,&system_memory);

    executor_t my_exec;
    executor_init(&my_exec, my_cpu_arr, CPU_KOP);

    clk_clock_init(&clkm_1);
    clk_timer_init(&tmr1_scheduler,&clkm_1, 
        SCHEDULER_FREQUENCY, &sched_execute, &my_scheduler);
    /*clk_timer_init(&tmr2_prgen,&clkm_1, 
        PROCESSGEN_FREQUENCY,&prgen_generate,&my_pr_generator);*/
    clk_timer_init(&tmr2_prgen,&clkm_1, 
        PROCESSGEN_FREQUENCY,&loader_loadNextProcess,&my_loader);
    clk_timer_init(&tmr3_executor,&clkm_1, 
        1,&executor_exec,&my_exec);
        
    clk_timer_init(&debug_tmr3_print_pqueue, &clkm_1, 
        EGOERA_PRINT_FREQUENCY, &sched_print_sched_State, &my_scheduler);

    sched_init(&my_scheduler, &tmr1_scheduler, my_cpu_arr,CPU_KOP,  MY_SCHED_PRIO_RR);
    prgen_init(&my_pr_generator, &my_scheduler, PRGEN_MAX_PID, PRGEN_MIN_PID, MAX_PRIO);

    
    if (pthread_create(&tr_tmr1_sched, NULL, clk_timer_start, (void *) &tmr1_scheduler))
        printf("Error: Timer started\n"); 
    if (pthread_create(&tr_tmr2_prgen, NULL, clk_timer_start, (void *) &tmr2_prgen))
        printf("Error: Timer started\n"); 
    if (pthread_create(&tr_tmr_print, NULL, clk_timer_start, (void *) &debug_tmr3_print_pqueue))
        printf("Error: Timer started\n"); 
    if (pthread_create(&tr_tmr3_exec, NULL, clk_timer_start, (void *) &tmr3_executor))
        printf("Error: Timer started\n"); 
    
    printf("Clock execute\n");
    if (pthread_create(&tr_clk, NULL, clk_clock_start, (void *)  &clkm_1))
        printf("Error: Clock started\n");
    
    pthread_join(tr_clk, NULL);
     
    exit(0);
}


