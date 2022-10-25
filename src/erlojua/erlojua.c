#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define TIMER_KOP   2;
#define SCHEDULER_FREQUENCY   1000;

void *timer_start(void *argv);
void timer_send_signal(int id);
void *clock_start(void *tmr_kop);

pthread_mutex_t mutex_clk = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_clk_ziklo = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_tmr_arreta = PTHREAD_COND_INITIALIZER;
int tmr_kop_tick_consumed = 0;

int main()
{ 
    pthread_t tr_clk, tr_tmr1, tr_tmr2;
    
    int timer_kop = TIMER_KOP;
    printf("Clock execute\n");
    if (pthread_create(&tr_clk, NULL, clock_start, (void *) &timer_kop)){
        printf("Error: Clock started\n");
    }
    
    int timer1_freq = SCHEDULER_FREQUENCY;
    if (pthread_create(&tr_tmr1, NULL, timer_start, (void *) &timer1_freq)){
        printf("Error: Timer started\n"); 
    }

    int timer2_freq = 30;
    if (pthread_create(&tr_tmr2, NULL, timer_start, (void *) &timer2_freq)){
        printf("Error: Timer started\n"); 
    }
    
-    pthread_join(tr_clk, NULL);
    //pthread_join(tr_tmr1, NULL); 
    
    exit(0);
}

void *clock_start(void *tmr_kop){
    int *p_timer_kop = (int *)tmr_kop;
    int done;
    pthread_mutex_lock(&mutex_clk); // Mutexa blokeatu
    while(1){
        sleep(1);
        tmr_kop_tick_consumed = 0;   
        printf("CLK: tick\n");
        while ( tmr_kop_tick_consumed < *p_timer_kop )
        {
            
            pthread_cond_wait(&cond_tmr_arreta, &mutex_clk );
        }
        pthread_cond_broadcast(&cond_clk_ziklo);
    }
}

void *timer_start(void *argv){
    
    int TIMER_FREQUENCY = * (int *) argv;
    int count = TIMER_FREQUENCY;
    pthread_mutex_lock(&mutex_clk); // zer gertatuko litzateke, lerro hau clock-aren mutex-lock-a baino lehen exekutatuko balitz?

    // printf("Whilera sartu timer");
    
    while(1){
        count -= 1;
        printf("TMR: count %d \n", count);
        if (count <= 0){
            timer_send_signal(TIMER_FREQUENCY);
            count = TIMER_FREQUENCY;
        }
        tmr_kop_tick_consumed++;
        pthread_cond_signal(&cond_tmr_arreta);
        pthread_cond_wait(&cond_clk_ziklo, &mutex_clk);
    }
}

void timer_send_signal(int id){
    printf("Signal sent: %d", id);
}


