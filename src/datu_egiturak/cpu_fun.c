// PCB:
#include <cpu_data_s.h>
#include <stdlib.h>
#include <assert.h>

#define TLB_MAX_SPACE 20

int cpu_t_init(struct cpu_s * p_cpu, int core_num, int hari_num_per_core, struct physycal_memory * ps){
    if (core_num <= 0){
        return 1;
    }

    p_cpu->core_kant = core_num;

    struct core_s ** my_core_arr; // core_s- struct-etara Punteroen array-a definitzeko
    my_core_arr = malloc(core_num * sizeof(struct core_s *));
    struct core_s * curr_core;

    for(int i = 0; i < core_num; i++){
        curr_core = (struct core_s*) malloc(sizeof(struct core_s));
        core_t_init(curr_core, hari_num_per_core, ps);
        my_core_arr[i] = curr_core;
    }
    
    p_cpu->core_arr = my_core_arr;

    return 0;
}

int core_t_init(struct core_s * p_core, int hari_num, struct physycal_memory * ps){
    if (hari_num <= 0){
        return 1;
    }
    struct core_hari_s ** my_hari_arr;
    struct core_hari_s * cur_hari;
    
    my_hari_arr = (struct core_hari_s ** ) malloc(hari_num * sizeof(struct core_hari_s *));
    for(int i=0; i < hari_num; i++){
        cur_hari = (struct core_hari_s*)malloc(sizeof(struct core_hari_s));
        hari_t_init(cur_hari, ps);
        my_hari_arr[i] = cur_hari;
    }

    p_core->hari_kant = hari_num;
    p_core->hari_arr=my_hari_arr;
    
    return 0;
}

int hari_t_init(struct core_hari_s * p_hari, struct physycal_memory * ps){ //TODO: Añadir mas parametros 
    p_hari->IR=0;
    p_hari->cc=0;
    p_hari->PC=0;
    p_hari->PTBR=0;
    p_hari->R;
    mmu_init(&(p_hari->mem_manag_unit), ps, TLB_MAX_SPACE);
}

int get_hari_kop(struct cpu_s *p_cpu, int *hari_kop)
{
    assert(p_cpu != NULL);
    *hari_kop = 0;
    for(int i = 0; i < p_cpu->core_kant; i++){
        *hari_kop += p_cpu->core_arr[i]->hari_kant;
    }
    return 0;
}
