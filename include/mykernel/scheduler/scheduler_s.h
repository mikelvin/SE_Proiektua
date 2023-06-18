#ifndef MY_SCHEDULER_H
#define MY_SCHEDULER_H

#include <pthread.h>
#include <lnklist_s.h>
#include <pcb_s.h>
#include <runqueues_s.h>
#include "../cpu_data_s.h"

#include <clk_s.h>

#include <scheduler_data_interface.h>

# define NULL_PROCESS_RUN 1
# define PROCESS_RUN 2


typedef struct sched_egitura sched_basic_t;

struct sched_egitura {
    pthread_mutex_t * sched_mutex_list; //Prozesuen lista era konkurrentean ez atzitzeko mutex-a. Prozesuen interfazeko PCB-ak gordetzen dituzten datu egituren integritatearen babes bezala jokatzen du.

    int current_politika; // Scheduler-ean kargatutako politikaren kodea gordetzen duen elementua.
    s_i_sched scheduler_queue_iface_object; //Scheduler interfaze objektua. Bertan PCB-ak manipulatzeko funtzioak eta datu egiturak eurkituko dira.

    pcb_t null_process; //Null process-aren PCB-a. Prozesu baliagarririk ez dagoenean CPU harían kargatuko den prozesua.

    clk_timer_s * timer; //Proiektuaren lehenengo zatian diseinatutako timer objektua. Quantum dekrementazioaren indizea lortzeko erabiliko da. Timer hau, scheduler-aren akribazioa kudetuko duen timer-a izan beharko da.

    struct sched_cpuThreadControl ** coreControl_array; //Core hari guztien egoerak kontrolatzen dituzten objektuen array-a.
    int coreControl_arr_len; // "coreControl_array" array-aren luzeera.
    
};

struct sched_cpuThreadControl{
    struct core_hari_s * core_hari_s;   // Kontrolatuko den CPU hariaren erreferentzia
    pcb_t * current_running_process;    // CPU harian, une horretan exekutatzen hari den prozesuaren PCB-a.
    int cur_process_quantum;            // Une honetan exekutatzen hari den prozesuaren quantum-a. Balio honek, prozesuari geratzen zaion quantum denbora adieraziko du.
    int current_cpu_SchedStatus;        // CPU hariaren egoera adierazten duen aldagaia. Null prozesua exekutatzen ari den ala ez adierazteko erabiliko da gehienbat.
};

void sched_AddToProcessQueue(sched_basic_t * sched, pcb_t * new_pcb);
void sched_execute(sched_basic_t * sched);
void sched_Scheduler_Dispatcher(sched_basic_t * sched, struct sched_cpuThreadControl * thr_control);
pcb_t * __sched_GetNextFromProcessQueue(sched_basic_t * sched);

void sched_init(sched_basic_t *sched, clk_timer_s *timer, struct cpu_s my_cpu_arr[], int cpu_kop, int SCHED_POLITIKA);

void __disp_load(struct core_hari_s * cpu_core_hari, pcb_t * pcb);
void __disp_unload(struct core_hari_s * cpu_core_hari, pcb_t * pcb);
void disp_Dispatch(pcb_t * old_pcb, pcb_t * new_pcb, struct core_hari_s * cpu_core_hari);
void disp_Terminate(struct core_hari_s * cpu_core_hari,pcb_t * pcb);
pcb_t * __sched_getNextProcessAndCleanQueue(sched_basic_t * sched, struct sched_cpuThreadControl * thread_control);

int sched_cpuThreadControl_init(struct sched_cpuThreadControl* tr_ctrl, core_hari_s * core_hari_s, pcb_t * cur_process, int cur_process_quantum, int current_cpu_SchedStatus);

/**
 * @brief Funtzioa exekutatzen den momentuan "thr_control"-ean exekutatzen ari den 
 * prozesuaren PID-a eguneratzen du. Prozesua exekuzioan dagoela suposatzen da.
 * 
 * @pre thr_control->current_running_process ezin da NULL izan
 * @param thr_control 
 * @return int 
 */
int __sched_updateRunning_pcb(struct sched_cpuThreadControl * thr_control);

#endif
