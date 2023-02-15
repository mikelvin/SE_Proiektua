#ifndef CPU_DATA_H
#define CPU_DATA_H

# include <stdint.h>
# include <memory_s.h>

struct cpu_s {
    int core_kant;
    struct core_s ** core_arr;
};

struct core_s {
    int hari_kant;
    struct core_hari_s ** hari_arr;
};

struct core_hari_s {
    uint32_t PC; // Program Conter Erregistroa 
    uint32_t IR; //
    uint32_t PTBR; 

    //Erregistro orokorrak
    int32_t R[16];
    int32_t cc;
    struct mmu mem_manag_unit;
};



int cpu_t_init(struct cpu_s * p_cpu, int core_num, int hari_num_per_core);

int core_t_init(struct core_s * p_core, int hari_num);

int hari_t_init(struct core_hari_s * p_hari);

uint32_t core_resolve(struct core_hari_s * core_h, uint32_t virt_adress);

uint32_t core_execute(struct core_hari_s * core_h);

uint32_t core_execute_agindu(struct core_hari_s * core_h, uint32_t agindu_value);

#endif