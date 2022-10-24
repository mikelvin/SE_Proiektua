#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define TIMER_KOP   1;

void *timer_start();
void timer_send_signal();
void *clock_start(void *vargp);

pthread_mutex_t mutex_clk = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_clk_ziklo = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_tmr_arreta = PTHREAD_COND_INITIALIZER;

int main()
{
    printf("start\n");
    
    pthread_t tr_clk, tr_tmr1, tr_tmr2;

    int timer_ticks = 3000;
    int timer_kop = TIMER_KOP;
    printf("Clock started1\n");
    if (pthread_create(&tr_clk, NULL, clock_start, (void *) &timer_kop)){
        printf("Error: Clock started\n");
    }
    sleep(5);
    printf("Timer started1\n");
    if (pthread_create(&tr_tmr1, NULL, timer_start, (void *) &timer_ticks)){
        printf("Error: Timer started"); 
    }
    
    pthread_join(tr_clk, NULL);
    //pthread_join(tr_tmr1, NULL); 
    
    exit(0);
}


void *clock_start(void *vargp){
    printf("Hey im ck2");
    int *p_timer_kop = (int *)vargp;
    int done;
    while(1){
        done = 0;
        // printf("Whilera sartu da clock");
        pthread_mutex_lock(&mutex_clk); // Mutexa blokeatu
        while ( done < *p_timer_kop )
        {
            pthread_cond_wait(&cond_tmr_arreta, &mutex_clk );
            done++;
        }
        pthread_cond_broadcast(&cond_clk_ziklo);
        pthread_mutex_unlock(&mutex_clk);
    }
}

void *timer_start(void *argv){
    // printf("Timer started2");
    int *count = (int *) argv;
    pthread_mutex_lock(&mutex_clk); // zer gertatuko litzateke, lerro hau clock-aren mutex-lock-a baino lehen exekutatuko balitz?
    // printf("Whilera sartu timer");
    
    while(1){
        count--;
        if (count < 0){
            timer_send_signal();
            count=(int *) argv;
        }

        pthread_cond_signal(&cond_tmr_arreta);
        pthread_cond_wait(&cond_clk_ziklo, &mutex_clk);
    }
}

void timer_send_signal(){
    printf("Signal sent");
}


