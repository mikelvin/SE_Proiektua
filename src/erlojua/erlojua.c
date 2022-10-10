#include <pthread.h>
#include <signal.h>

#define TIMER_KOP   2;

void *timer_start();

pthread_mutex_t mutex_clk = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_clk_ziklo = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_tmr_arreta = PTHREAD_COND_INITIALIZER;

int timer_ticks = 15;

int main(){

    pthread_t tr_tmr1, tr_tmr2;

    int done;
    while(1){
        done = 0;
        pthread_mutex_lock(&mutex_clk);
        while ( done < TIMER_KOP )
        {
            pthread_cond_wait(&cond_tmr_arreta, &mutex_clk );
            done++;

        }
        
    }
}

void *timer_start(){
    int count = timer_ticks;
    pthread_mutex_lock(&mutex_clk);
    while(1){
        count--;

        if (count < 0){
            kill()
        }

        pthread_cond_wait(&cond_clk_ziklo, &mutex_clk);



    }
}

void timer_send_signal(){

}
