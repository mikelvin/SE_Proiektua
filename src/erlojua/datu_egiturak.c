// PCB:
#include <stdlib.h>
#include <stdio.h>
#include <cpu_data_s.h>

int cpu_t_init(struct cpu_s * p_cpu, int core_num, int hari_num_per_core){
    if (core_num <= 0){
        return 1;
    }

    p_cpu->core_kant = core_num;

    struct core_s ** my_core_arr; // core_s- struct-etara Punteroen array-a definitzeko
    my_core_arr = malloc(core_num * sizeof(struct core_s *));
    struct core_s * curr_core;

    for(int i; i < core_num; i++){
        curr_core = (struct core_s*)malloc(sizeof(struct core_s));
        core_t_init(curr_core, hari_num_per_core);
        my_core_arr[i] = curr_core;
    }
    
    p_cpu->core_arr = my_core_arr;

    return 0;
}

int core_t_init(struct core_s * p_core, int hari_num){
    if (hari_num <= 0){
        return 1;
    }
    struct core_hari_s ** my_hari_arr;
    struct core_hari_s * cur_hari;
    
    my_hari_arr = malloc(hari_num * sizeof(struct core_hari_s *));
    for(int i; i < hari_num; i++){
        cur_hari = (struct core_hari_s*)malloc(sizeof(struct core_hari_s));
        hari_t_init(cur_hari);
        my_hari_arr[i] = cur_hari;
    }

    p_core->hari_kant = hari_num;
    p_core->hari_arr=my_hari_arr;
    
    return 0;
}

int hari_t_init(struct core_hari_s * p_hari){
    p_hari->pid=0;
}