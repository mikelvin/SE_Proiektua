# ifndef __MY_EXECUTOR__
# define __MY_EXECUTOR__

#include "../mykernel/cpu_data_s.h"
#include <inttypes.h>

#define OP_LD 0
#define OP_ST 1
#define OP_ADD 2
#define OP_SUB 3
#define OP_MUL 4
#define OP_DIV 5
#define OP_AND 6
#define OP_OR 7
#define OP_XOR 8
#define OP_MOV 9
#define OP_CMP 10
#define OP_B 11
#define OP_BEQ 12
#define OP_BGT 13
#define OP_BLT 14
#define OP_EXIT 15

#define EXIT_CODE 0xF0000000

typedef struct executor_t{
    cpu_s * cpu_arr;
    int cpu_arr_len;
} executor_t;

uint32_t getNibbleRange(uint32_t item, int leftmost_nibble, int rightmost_nibble);
/** INTERFACE */
int executor_init(executor_t * exec, cpu_s cpu_arr[], int cpu_arr_len);
void executor_exec(executor_t * exec);
/** --------- */

int execute_all(cpu_s cpu_arr[], int cpu_arr_len);
int execute_current(struct core_hari_s *cpu_context);

void __ex_MEM(struct core_hari_s *cpu_context, uint32_t plain_command);

int __execute(struct core_hari_s *cpu_context, uint32_t plain_command);

void __ex_MEM_ld(struct core_hari_s *cpu_context, uint8_t reg, uint32_t virt_adress);

void __ex_MEM_st(struct core_hari_s *cpu_context, uint8_t reg, uint32_t virt_adress);


void __ex_ALU(struct core_hari_s *cpu_context, uint32_t plain_command);

void __ex_arithm_add(struct core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_arithm_sub(struct core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_arithm_mul(struct core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_arithm_div(struct core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_logic_and(struct core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_logic_or(struct core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_logic_xor(struct core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_arithm_mov(struct core_hari_s *cpu_context, uint8_t targ, uint8_t op);

void __ex_arithm_cmp(struct core_hari_s *cpu_context, uint8_t targ, uint8_t op);


void __ex_CU(struct core_hari_s *cpu_context, uint32_t plain_command);

void __ex_b(struct core_hari_s *cpu_context, uint32_t command);

void __ex_beq(struct core_hari_s *cpu_context, uint32_t command);

void __ex_bgt(struct core_hari_s *cpu_context, uint32_t command);

void __ex_blt(struct core_hari_s *cpu_context, uint32_t command);

void __ex_exit(struct core_hari_s *cpu_context, uint32_t command);

#endif
