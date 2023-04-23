#ifndef LOADER_S_H
#define LOADER_S_H
#include <lnklist_s.h>
#include <pcb_s.h>
#include <memory_s.h>
#include <scheduler_s.h>


#define LOAD_DIR "/home/kilme/UNI/SE/sheduler_proj/data/pr_test/"

typedef struct loader_s {
    lnklist_LFRL unloaded_elf_list;
    int prid_counter;
    int max_prio;
    struct mmu mmu;
    sched_basic_t * sched;
} loader_s;

int __loader_getValidPrio(loader_s * loader);
int __loaden_getValidPid(loader_s * loader);

int loader_startup(loader_s * loader, struct physycal_memory *ps, sched_basic_t * sched, int max_prio);
int loader_loadNextProcess(loader_s * loader);


/**
    LOAD_DIR konstantean definituta dagoen direktorioan dagoen programa guztien path-ak biltzen ditu ondoren 
    
*/
int __load_file(struct mmu * p_mmu, char * d_name, struct pcb_str * loading_pcb);

int loadNullPCB(struct mmu * p_mmu, struct pcb_str * loading_pcb);

/**
 * @brief 8 karaktereko String batean kodetuta dauden 4 byteko (32bit) hitzaren uint32 balioa jaso
 * 
 * @param hexStringLen8 8 karaktere edo gutxiko String-a. Bertan HEX formatuan 4 byte kodetuta egon beharko dira. 8 karaktere baino gehiago zehazten badira, errorea jasoko da.
 * @param intVal 
 * @return int 
 */
int __hexString2int32(char * hexString, uint32_t * intVal);

/**
 * @brief ASCII-n kodetuta dagoen Hex karaktere baten integer balio erreala bueltatu.
 * 
 * Hau da da, \c ASCII-n '0' izango zenari, 0 \c int balioa eman. Eta 'F' izango zenari, 16 balioa eman.
 * 
 * @param hexChar ASCII-n kodetuta dagoen HEX karakterea
 * @param intVal HEX balioaren integer balio erreala.
 * @return \c int Success egoera
 */
int __hexChar2int4(char hexChar , uint8_t * intVal);

#endif