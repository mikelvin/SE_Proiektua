#ifndef LOADER_S_H
#define LOADER_S_H
#include <lnklist_s.h>
#include <pcb_s.h>
#include <memory_s.h>


#define LOAD_DIR "/home/kilme/UNI/SE/sheduler_proj/data/pr_test/"

/**
    LOAD_DIR konstantean definituta dagoen direktorioan dagoen programa guztiak kargatzen ditu memorian.
    @param p_mmu Memoria erreserbatzeko erabiliko den mmu-a, memoria fisikora atzpena duena
    
*/
int loader(struct mmu * p_mmu, lnklist_LFRL * pcb_list);

int load_file(struct mmu * p_mmu, char * d_name, struct pcb_str * loading_pcb);

int load_line();

#endif