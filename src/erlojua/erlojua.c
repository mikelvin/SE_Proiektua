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

    int timer_ticks = 10;
    int timer_kop = TIMER_KOP;
    printf("Clock execute\n");
    if (pthread_create(&tr_clk, NULL, clock_start, (void *) &timer_kop)){
        printf("Error: Clock started\n");
    }
    sleep(5);
    printf("Timer execute\n");
    if (pthread_create(&tr_tmr1, NULL, timer_start, (void *) &timer_ticks)){
        printf("Error: Timer started\n"); 
    }
    
-    pthread_join(tr_clk, NULL);
    //pthread_join(tr_tmr1, NULL); 
    
    exit(0);
}


void *clock_start(void *vargp){
    printf("Hey im clk\n");
    int *p_timer_kop = (int *)vargp;
    int done;
    printf("CLK: Im going to lock MUTEX\n");
    pthread_mutex_lock(&mutex_clk); // Mutexa blokeatu
    printf("CLK: MUTEX LOCKED for me\n");
    while(1){
        sleep(1);
        done = 0;
        // printf("Whilera sartu da clock");
        
        while ( done < *p_timer_kop )
        {
            printf("CLK: Im releasing MUTEX (UNLOCK)\n");
            printf("CLK: Im waiting -> cond_tmr_arreta\n");
            pthread_cond_wait(&cond_tmr_arreta, &mutex_clk );
            printf("CLK: End Wait --> cond_tmr_arreta recived\n");
            printf("CLK: Im locked\n");
            done++;
        }
        printf("CLK: Sending broadcast -> cond_clk_ziklo\n");
        pthread_cond_broadcast(&cond_clk_ziklo);
        printf("CLK: Broadcast sent -> cond_clk_ziklo\n");
        // pthread_mutex_unlock(&mutex_clk);
    }
}

void *timer_start(void *argv){
    printf("Hey im timer\n");
    int *count = (int *) argv;

    printf("TIMER: Im going to lock\n");
    pthread_mutex_lock(&mutex_clk); // zer gertatuko litzateke, lerro hau clock-aren mutex-lock-a baino lehen exekutatuko balitz?
    printf("TIMER: Im locked\n");
    // printf("Whilera sartu timer");
    
    while(1){
        sleep(1);
        count--;
        if (count < 0){
            timer_send_signal();
            count=(int *) argv;
        }
        printf("TIMER: Sending signal -> cond_tmr_arreta\n");
        pthread_cond_signal(&cond_tmr_arreta);
        printf("TIMER: Signal sent -> cond_tmr_arreta\n");

        printf("TIMER: Im UNlocked\n");
        printf("TIMER: Im waiting -> cond_clk_ziklo\n");
        pthread_cond_wait(&cond_clk_ziklo, &mutex_clk);
        printf("TIMER: End Wait --> cond_clk_ziklo recived\n");
        printf("TIMER: Im locked\n");
    }
}

void timer_send_signal(){
    printf("Signal sent");
}


