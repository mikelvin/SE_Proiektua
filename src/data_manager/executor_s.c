
# define PC_INCREMENT 4

#include "executor.h"

/**
 * @brief item aldagaiaren zati bat lortu. Zati hauek adierazteko, nibble (4 bit) tarte bat adieraziko da.
 * Tarte honen goi zein behe mugak barne kontuan hartuko dira.
 * 
 * @param item 
 * @param leftmost_nibble 
 * @param rightmost_nibble 
 * @return uint32_t 
 */
uint32_t getNibbleRange(uint32_t item, int leftmost_nibble, int rightmost_nibble){
    item = item >> rightmost_nibble * 4;
    item = item & ((1 << (4 + (leftmost_nibble - rightmost_nibble)*4)) - 1);
    return item;
}
int executor_init(executor_t * exec, cpu_s cpu_arr[], int cpu_arr_len){
    exec->cpu_arr = cpu_arr;
     exec->cpu_arr_len = cpu_arr_len;
}

void executor_exec(executor_t * exec){
    execute_all(exec->cpu_arr, exec->cpu_arr_len);
}

int execute_all(cpu_s cpu_arr[], int cpu_arr_len){
    for(int i = 0; i < cpu_arr_len; i++)
    {
        int current_core_kop = cpu_arr[i].core_kant;
        struct core_s ** current_core_arr = cpu_arr[i].core_arr;
        for(int j = 0; j < current_core_kop; j++)
        {
            int current_hari_kop = current_core_arr[j]->hari_kant;
            for(int k = 0 ; k < current_hari_kop; k++)
            {
                execute_current(current_core_arr[j]->hari_arr[k]);
            }
        }
    } 
}

int execute_current(struct core_hari_s * cpu_context){
    uint32_t plain_command_adress = mmu_resolve(&cpu_context->mem_manag_unit, cpu_context->PTBR, cpu_context->PC);
    uint32_t plain_command = pm_read_word(cpu_context->mem_manag_unit.ps, plain_command_adress);
    cpu_context->IR = plain_command;
    return __execute(cpu_context, plain_command);
}

/**
 * @brief 32 bit-eko formatuan kodetutako agindu bat exekutazen du CPU-aren kontextuan
 * 
 * @param cpu_context Egindua exekutatu nahi den CPU-a 
 * @param command Exekutatu nahi den agindua
 * @return int 
 */
int __execute(struct core_hari_s * cpu_context, uint32_t plain_command){
    uint8_t command = getNibbleRange(plain_command, 7, 7);
    
    switch (command)
    {
    case OP_LD:  // ld
    case OP_ST:  // st
        cpu_context->PC = cpu_context->PC + PC_INCREMENT;
        __ex_MEM(cpu_context, plain_command);
        break;
    case OP_ADD:
    case OP_SUB:
    case OP_MUL:
    case OP_DIV:
    case OP_AND:
    case OP_OR:
    case OP_XOR:
    case OP_MOV:
    case OP_CMP:
        cpu_context->PC = cpu_context->PC + PC_INCREMENT;
        __ex_ALU(cpu_context, plain_command);
        break;
    case OP_B: 
    case OP_BEQ:
    case OP_BGT: 
    case OP_BLT: 
        cpu_context->PC = cpu_context->PC + PC_INCREMENT;
    case OP_EXIT: 
        __ex_CU(cpu_context, plain_command);
        break;
    default:
        return -1;
        break;
    }
    return 0;
}

void __ex_MEM_ld(struct core_hari_s * cpu_context, uint8_t reg, uint32_t virt_adress){
    uint32_t phis_addr = mmu_resolve(&cpu_context->mem_manag_unit,cpu_context->PTBR, virt_adress);
    uint32_t word = pm_read_word(cpu_context->mem_manag_unit.ps, phis_addr);
    cpu_context->R[reg] = *((int32_t *) &word); //TODO: honek ez ditu datuak eraldatuko?

}
void __ex_MEM_st(struct core_hari_s * cpu_context, uint8_t reg, uint32_t virt_adress){
    uint32_t phis_addr = mmu_resolve(&cpu_context->mem_manag_unit,cpu_context->PTBR, virt_adress);
    pm_write_word(cpu_context->mem_manag_unit.ps, phis_addr, (uint32_t) cpu_context->R[reg]);
}

void __ex_MEM(struct core_hari_s * cpu_context, uint32_t plain_command){
    uint8_t command = getNibbleRange(plain_command, 7, 7);
    uint8_t targ_reg = getNibbleRange(plain_command, 6, 6);
    uint32_t virt_addr = getNibbleRange(plain_command, 5, 0);
    switch (command)
    {
    case 0:
        __ex_MEM_ld(cpu_context, targ_reg, virt_addr);
        break;
    case 1:
        __ex_MEM_st(cpu_context, targ_reg, virt_addr);
        break;
    default:
        break;
    }
}

void __ex_ALU(struct core_hari_s * cpu_context, uint32_t plain_command){
    uint8_t command = getNibbleRange(plain_command, 7, 7);
    uint8_t targ_reg = getNibbleRange(plain_command, 6, 6);
    uint32_t oper_reg1 = getNibbleRange(plain_command, 5, 5);
    uint32_t oper_reg2 = getNibbleRange(plain_command, 4, 4);
    switch (command)
    {
    case 2:
        __ex_arithm_add(cpu_context,targ_reg, oper_reg1,oper_reg2);
        break;
    case 3:
        __ex_arithm_sub(cpu_context,targ_reg, oper_reg1, oper_reg2);
        break;
    case 4:
        __ex_arithm_mul(cpu_context, targ_reg, oper_reg1, oper_reg2);
        break;
    case 5:
        __ex_arithm_div(cpu_context, targ_reg, oper_reg1, oper_reg2);
        break;
    case 6:
        __ex_logic_and(cpu_context, targ_reg, oper_reg1, oper_reg2);
        break;
    case 7:
        __ex_logic_or(cpu_context, targ_reg, oper_reg1, oper_reg2);
        break;
    case 8:
        __ex_logic_xor(cpu_context, targ_reg, oper_reg1, oper_reg2);
        break;
    case 9:
        __ex_arithm_mov(cpu_context, targ_reg, oper_reg1);
        break;
    case 10:
        __ex_arithm_cmp(cpu_context, targ_reg, oper_reg1);
        break;
    default:
        break;
    }
}
void __ex_arithm_add(struct core_hari_s * cpu_context, uint8_t targ, uint8_t op1, uint8_t op2){
    cpu_context->R[targ] = cpu_context->R[op1] + cpu_context->R[op2];
}
void __ex_arithm_sub(struct core_hari_s * cpu_context, uint8_t targ, uint8_t op1, uint8_t op2){
    cpu_context->R[targ] = cpu_context->R[op1] - cpu_context->R[op2];
}
void __ex_arithm_mul(struct core_hari_s * cpu_context, uint8_t targ, uint8_t op1, uint8_t op2){
    cpu_context->R[targ] = cpu_context->R[op1] * cpu_context->R[op2];
}
void __ex_arithm_div(struct core_hari_s * cpu_context, uint8_t targ, uint8_t op1, uint8_t op2){
    cpu_context->R[targ] = cpu_context->R[op1] / cpu_context->R[op2];
}
void __ex_logic_and(struct core_hari_s * cpu_context, uint8_t targ, uint8_t op1, uint8_t op2){
    cpu_context->R[targ] = cpu_context->R[op1] & cpu_context->R[op2];
}
void __ex_logic_or(struct core_hari_s * cpu_context, uint8_t targ, uint8_t op1, uint8_t op2){
    cpu_context->R[targ] = cpu_context->R[op1] | cpu_context->R[op2];
}
void __ex_logic_xor(struct core_hari_s * cpu_context, uint8_t targ, uint8_t op1, uint8_t op2){
    cpu_context->R[targ] = cpu_context->R[op1] ^ cpu_context->R[op2];
}
void __ex_arithm_mov(struct core_hari_s * cpu_context, uint8_t targ, uint8_t op){
    cpu_context->R[targ] = cpu_context->R[op];
}
void __ex_arithm_cmp(struct core_hari_s * cpu_context, uint8_t targ, uint8_t op){
    cpu_context->cc = cpu_context->R[targ] - cpu_context->R[op]; 
    //TODO: No se como gestionar el CC ya que mi programa trabaja con unsigned ints

}
void __ex_CU(struct core_hari_s * cpu_context, uint32_t plain_command){
    uint8_t command = getNibbleRange(plain_command, 7, 7);
    switch (command)
    {
    case 11:
        __ex_b(cpu_context,plain_command);
        break;
    case 12:
        __ex_beq(cpu_context,plain_command);
        break;
    case 13:
        __ex_bgt(cpu_context,plain_command);
        break;
    case 14:
        __ex_blt(cpu_context,plain_command);
        break;
    case 15:
        __ex_exit(cpu_context,plain_command);
        break;
    }
}

void __ex_b(struct core_hari_s * cpu_context, uint32_t command){
    uint32_t adress = getNibbleRange(command, 5, 0);
    cpu_context->PC = adress;
    // TODO: Puede ser que el adress que se cargue no este dentro de la memoria accesible por el programa
    
}
void __ex_beq(struct core_hari_s * cpu_context, uint32_t command){
    if(cpu_context->cc == 0){
        __ex_b(cpu_context, command);
    }
}
void __ex_bgt(struct core_hari_s * cpu_context, uint32_t command){
    if(cpu_context->cc > 0){
        __ex_b(cpu_context, command);
    }
}
void __ex_blt(struct core_hari_s * cpu_context, uint32_t command){
    if(cpu_context->cc < 0){
        __ex_b(cpu_context, command);
    }
}
void __ex_exit(struct core_hari_s * cpu_context, uint32_t command){

}



