#ifndef CPU_DATA_H
#define CPU_DATA_H

# include <stdint.h>
# include "../data_manager/memory_s.h"

#define CPU_HARI_REG_KOP 16

struct cpu_s {
    int core_kant;
    struct core_s ** core_arr;
};
typedef struct cpu_s cpu_s;

struct core_s {
    int hari_kant;
    struct core_hari_s ** hari_arr;
};
typedef struct core_s core_s;

struct core_hari_s {
    uint32_t PC; // Program Conter Erregistroa 
    uint32_t IR; //
    uint32_t PTBR; 

    //Erregistro orokorrak
    int32_t R[CPU_HARI_REG_KOP];
    int32_t cc;
    struct mmu mem_manag_unit;
};
typedef struct core_hari_s core_hari_s;


int cpu_t_init(struct cpu_s * p_cpu, int core_num, int hari_num_per_core, struct physycal_memory * ps);

int core_t_init(struct core_s * p_core, int hari_num, struct physycal_memory * ps);

int hari_t_init(struct core_hari_s * p_hari, struct physycal_memory * ps);

/**
 * @brief CPU batek, dituen core guztiak kontuan hartuta, core guzti hauek dituzten core_hari kopuru totala lortu.
 * 
 * @param[in] p_cpu CPU target
 * @param[out] hari_kop Cpu-ak dituen hari kopurua
 * @return int errore kodea
 */
int get_hari_kop(struct cpu_s * p_cpu, int * hari_kop);

uint32_t core_resolve(struct core_hari_s * core_h, uint32_t virt_adress);

uint32_t core_execute(struct core_hari_s * core_h);

uint32_t core_execute_agindu(struct core_hari_s * core_h, uint32_t agindu_value);

#endif