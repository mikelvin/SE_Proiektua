# ifndef __MY_EXECUTOR__
# define __MY_EXECUTOR__

#include <cpu_data_s.h>
#include <inttypes.h>

uint32_t getNibbleRange(uint32_t item, int leftmost_nibble, int rightmost_nibble);

int execute_current(struct core_hari_s *cpu_context);

void __ex_MEM(core_hari_s *cpu_context, uint32_t plain_command);

int __execute(struct core_hari_s *cpu_context, uint32_t plain_command);

void __ex_MEM_ld(struct core_hari_s *cpu_context, uint8_t reg, uint32_t virt_adress);

void __ex_MEM_st(struct core_hari_s *cpu_context, uint8_t reg, uint32_t virt_adress);


void __ex_ALU(core_hari_s *cpu_context, uint32_t plain_command);

void __ex_arithm_add(core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_arithm_sub(core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_arithm_mul(core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_arithm_div(core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_logic_and(core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_logic_or(core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_logic_xor(core_hari_s *cpu_context, uint8_t targ, uint8_t op1, uint8_t op2);

void __ex_arithm_mov(core_hari_s *cpu_context, uint8_t targ, uint8_t op);

void __ex_arithm_cmp(core_hari_s *cpu_context, uint8_t targ, uint8_t op);


void __ex_CU(core_hari_s *cpu_context, uint32_t plain_command);

void __ex_b(core_hari_s *cpu_context, uint32_t command);

void __ex_beq(core_hari_s *cpu_context, uint32_t command);

void __ex_bgt(core_hari_s *cpu_context, uint32_t command);

void __ex_blt(core_hari_s *cpu_context, uint32_t command);

void __ex_exit(core_hari_s *cpu_context, uint32_t command);

#endif
