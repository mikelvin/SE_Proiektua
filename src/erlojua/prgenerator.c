#include <pcb_s.h>
#include <prgenerator.h>
#include <stdio.h>
#include <stdlib.h>

void prgen_init(pr_gen_t *prgen, sched_basic_t * sched, int max_PID, int min_PID, int max_prio){
    prgen->MAX_PID = max_PID;
    prgen->MIN_PID = min_PID;
    prgen->scheduler = sched;
    prgen->current_counter = 0;
    prgen->max_prio = max_prio;
}

int _prgen_getValidPid(pr_gen_t * pr_gen){
    //TODO: Frogatu scheduler-ean pid hori jada esleituta dagoen ala ez
    int new_pr_id;
    new_pr_id = rand() % (pr_gen->MAX_PID-pr_gen->MIN_PID) + pr_gen->MIN_PID;
    return new_pr_id;
}

int _prgen_getValidPrio(pr_gen_t * pr_gen){
    return (rand() % (pr_gen->max_prio));
}

void prgen_generate(pr_gen_t * pr_gen){
    printf("Process generator called: ");

    pcb_t * new_process_pcb;
    new_process_pcb = (pcb_t *) malloc(sizeof(pcb_t));

    
    pcb_init(new_process_pcb,_prgen_getValidPid(pr_gen),_prgen_getValidPrio(pr_gen));


    sched_AddToProcessQueue(pr_gen->scheduler, new_process_pcb);
}