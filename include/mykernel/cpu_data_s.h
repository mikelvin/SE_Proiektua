#ifndef CPU_DATA_H
#define CPU_DATA_H

struct cpu_s {
    int core_kant;
    struct core_s ** core_arr;
};

struct core_s {
    int hari_kant;
    struct core_hari_s ** hari_arr;
};

struct core_hari_s {
    int pid; // PlaceholderS
};

int cpu_t_init(struct cpu_s * p_cpu, int core_num, int hari_num_per_core);

int core_t_init(struct core_s * p_core, int hari_num);

int hari_t_init(struct core_hari_s * p_hari);

#endif