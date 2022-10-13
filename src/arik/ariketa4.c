#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

void *print_my_pid( void *ptr);

main()
{
    
    int child_kop = 3;
    pthread_t thread_list[child_kop];

    int i;
    for(i = 0; i < child_kop; i++){
        pthread_create(&thread_list[i], NULL, print_my_pid, NULL);
    }
    for (i = 0; i < child_kop; i++){
        pthread_join(thread_list[i], NULL);
    }
}

void *print_my_pid(void *ptr){
    printf("My process ID is %d \n", getpid());
    return 0;
}
