#include <memory_s.h>
#include <err.h>
#include <stdlib.h>
#include <assert.h>
#include <lnklist_functionalities_s.h>

uint32_t pm_read_word(struct physycal_memory * ps, uint32_t phys_adress){
    const uint32_t word_start_address_mask = (BUS_ADRESS_SPACE-1) & ~(BUS_DATA_BYTES_SIZE-1);

    uint32_t word_start_adress = phys_adress & word_start_address_mask;

    uint32_t data = 0x0;
    for(int byte_i = 0; byte_i < BUS_DATA_BYTES_SIZE; byte_i++){
        data = data << DATA_CELL_STORE_BYTES*8;
        uint8_t current_byte = ps->memory[word_start_adress + byte_i];
        data = data | (current_byte & 0xFF);
    }

    return data;
}

int pm_write_word(struct physycal_memory * ps, uint32_t phys_adress, uint32_t word ){
    const uint32_t word_start_address_mask = (BUS_ADRESS_SPACE-1) & ~(BUS_DATA_BYTES_SIZE-1);
    uint32_t word_start_adress = phys_adress & word_start_address_mask;

    uint32_t data = word;
    for(int byte_i = BUS_DATA_BYTES_SIZE-1; byte_i >= 0; byte_i--){
        uint32_t current_byte = data & ((1<<DATA_CELL_STORE_BITS)-1); // data & 0x0000$00FF 
        ps->memory[word_start_adress + byte_i] = current_byte;
        data = data >> DATA_CELL_STORE_BITS;
    }
    return 1;
}

int __search_space4free(void * free_block, void * search_args){
    struct free_block_search_args * fb_args = (struct free_block_search_args *) search_args;
    int default_frame_kop = 0;
    if(!search_args){
        search_args = &default_frame_kop;
    }
    struct free_block * fb = (struct free_block *) free_block;
    if(!fb){
        return 0;
    }

    if(fb->end_adress < fb_args->start_limit_adress || fb_args->end_limit_adress < fb->start_adress){
        return 0;
    }
    
    uint32_t valid_start = fb->start_adress;
    uint32_t valid_end = fb->end_adress;
    if(fb->start_adress < fb_args->start_limit_adress){
        valid_start = fb_args->start_limit_adress;
    }
    if(fb_args->end_limit_adress < fb->end_adress){
        valid_end = fb_args->end_limit_adress;
    }
    return (valid_start - valid_end + 1) >= fb_args->min_adress_kont;

}   

/*
    Funcion para buscar si un determinado adress esta dentro de algun free_space
*/
int __search_for_adress(void * free_block, void * search_args){
    uint32_t * adress = (int *) search_args;
    if(!search_args){
        return 0;
    }
    struct free_block * fb = (struct free_block *) free_block;
    if(!fb){
        return 0;
    }

    if(fb->start_adress >= *adress && fb->end_adress < *adress){
        return 1;
    }else{
        return 0;
    }

}

int pm_m_frame_malloc(struct physycal_memory * ps, uint32_t * adress){
    
    struct free_block_search_args fb_args = {
        .min_adress_kont = FRAME_SIZE,
        .start_limit_adress = MEMO_SPACE_START_ADDRESS,
        .end_limit_adress = MEMO_SPACE_END_ADDRESS};

    uint32_t resulting_adress;
    
    if( ! pm_memory_allocation_request(ps, &fb_args, &resulting_adress)){
        err(1, "Not enough free space for table allocation");
        return 0;
    };
    *adress = resulting_adress;
    return 1;
}

int pm_m_frame_free(struct physycal_memory * ps, uint32_t phys_adress){
    uint32_t frame_start_adress = phys_adress & FRAME_ADRESS_MASK;

    if(lnklst_LFRL_peekFirstMatchFromRear(&ps->free_blocks, __search_for_adress, (void *) &frame_start_adress)){
        return 0;
    }

    struct free_block * new_fb;
    new_fb = (struct free_block *) malloc(sizeof(struct free_block));
    new_fb->start_adress = frame_start_adress;
    new_fb->end_adress = frame_start_adress + FRAME_SIZE -1;

    lnklst_LFRL_push(&ps->free_blocks, (void *) new_fb);
    return 1;
}

int pm_m_frame_malloc_bulk(struct physycal_memory * ps, uint32_t frame_adress_array[], int frame_kop){
    // TODO: 
    uint32_t frame_adress;
    int cur_entry;
    for(cur_entry = 0; cur_entry < frame_kop; cur_entry ++){
        if(!pm_m_frame_malloc(ps, &frame_adress_array[cur_entry])){
            break;
        }
    }
    
    // En el caso de que no pueda reservar todos los frames solicitados: 
    if(cur_entry != frame_kop){
        for(int i = 0; i < cur_entry; i ++){ // Libera todos los frames que han sido asignados
            pm_m_frame_free(ps, frame_adress_array[i]);
        }
        return 0;
    }

    return 1;
}

/*
    returns the PTBR of the page table allocated
*/
int pm_pt_page_table_malloc(struct physycal_memory * ps, uint32_t * adress,  uint32_t entries){
    struct free_block_search_args fb_args = {
        .min_adress_kont = PAGETABLE_CORE_ADRESS_KOP + PAGETABLE_CORE_ADRESS_KOP*entries,
        .start_limit_adress = KERNEL_SPACE_START_ADDRESS,
        .end_limit_adress = KERNEL_SPACE_END_ADDRESS};

    uint32_t resulting_adress;
    // Reservar la tabla
    if( ! pm_memory_allocation_request(ps, &fb_args, &resulting_adress)){
        err(1, "Not enough free space for table allocation");
        return 0;
    };
    uint32_t frame_adress_array[entries];
     if( ! pm_m_frame_malloc_bulk(ps, frame_adress_array, entries)){
        err(1, "Not enough free space for table allocation: not enough frame memo");
        return 0;
    };
    // Inicializar la tabla:
    //  - Longitud de la tabla
    pm_write_word(ps, resulting_adress, entries);
    //  - Reservar los frames
    uint32_t page_entry_adress = resulting_adress + PAGETABLE_CORE_ADRESS_KOP;
    
    for(int cur_entry = 0; cur_entry < entries; cur_entry ++){
        pm_write_word(ps, page_entry_adress, frame_adress_array[cur_entry]);
        page_entry_adress = page_entry_adress + PAGETABLE_ENTRY_ADRESS_KOP;
    }

    *adress = resulting_adress;
    return 1;
}

int  pm_pt_page_table_free(struct physycal_memory * ps, uint32_t adress){
    uint32_t table_start_adress = adress;

    if(lnklst_LFRL_peekFirstMatchFromRear(&ps->free_blocks, __search_for_adress, (void *) &table_start_adress)){
        return 0;
    }
    uint32_t entries = pm_read_word(ps, adress);
    
    struct free_block * new_fb;
    new_fb = (struct free_block *) malloc(sizeof(struct free_block));
    new_fb->start_adress = table_start_adress;
    new_fb->end_adress = table_start_adress + PAGETABLE_CORE_ADRESS_KOP + PAGETABLE_ENTRY_ADRESS_KOP * entries;
    lnklst_LFRL_push(&ps->free_blocks, (void *) new_fb);

    uint32_t current_pte_adress =  PAGETABLE_CORE_ADRESS_KOP;
    for(int i = 0; i < entries; i++){
        pm_m_frame_free(ps, pm_read_word(ps, current_pte_adress)); // TODO: POSIBLE FUTURO BUG Si se puede cambiar dinamicamente el tamaño de cada entrada de la page table (en este caso 4 bytes) --> entonces se deberia poder leer un numero de bytes determinado usando esta funcion y no solo word
        current_pte_adress = current_pte_adress + PAGETABLE_ENTRY_ADRESS_KOP;
    }
    return 1;
}


int pm_memory_allocation_request(struct physycal_memory * ps, struct free_block_search_args * request, uint32_t * ret_adress){
        
    int optimization_max_iterations = 1;
    int result;
    while(!(result = __pm_memory_allocation_procedure(ps, request, ret_adress)) && optimization_max_iterations){
        __optimize_free_block_list(ps);
        optimization_max_iterations--;
    }
    return result;
}

int __pm_memory_allocation_procedure(struct physycal_memory * ps, struct free_block_search_args * request, uint32_t * ret_adress){
    struct free_block * fb;
    
    fb = lnklst_LFRL_popFirstMatchFromRear(&ps->free_blocks, __search_space4free, request);
    
    if(!fb || !request || !ret_adress)
        return 0;
    
    uint32_t start_resulting_adress;

    // Define la direccion inicial a reservar
    if(fb->start_adress < request->start_limit_adress){ // [ <- A | target | C ] --> [ <b- A -b> | <- target | C ]
        start_resulting_adress = request->start_limit_adress; 
        struct free_block * memor_space_block = (struct free_block *) malloc(sizeof(struct free_block)) ; // [ <b- K -b> | M ]
        
        memor_space_block->start_adress = fb->start_adress;
        memor_space_block->end_adress = request->start_limit_adress - 1;

        lnklst_LFRL_push(&ps->free_blocks, (void *) memor_space_block);
    }else{
        start_resulting_adress = fb->start_adress; 
    }

    // Actualiza el bloque
    fb->start_adress = start_resulting_adress + request->min_adress_kont;
    if(fb->end_adress - fb->start_adress == 0){
        free(fb);
    }else if(fb->end_adress - fb->start_adress > 0){
        lnklst_LFRL_push(&ps->free_blocks, (void *) fb);
    }else{
        return 0;
    }

    *ret_adress = start_resulting_adress;
    return 1;
}

int __comparator_start_adress(void * a, void * b){
    struct free_block * fb_a = (struct free_block *) a;
    struct free_block * fb_b = (struct free_block *) b;

    if(fb_a->start_adress < fb_b->start_adress)
        return -1;
    else if(fb_a->start_adress = fb_b->start_adress)
        return 0;
    else
        return 1;
}



int __optimize_free_block_list(struct physycal_memory * ps){
    MergeSort(&ps->free_blocks, __comparator_start_adress);

    struct lnk_node * current = ps->free_blocks.first;
    
    struct free_block * cur_fb, * next_fb;

    while(current && current->next){
        cur_fb = (struct free_block *) current->data;
        next_fb = (struct free_block *) current->next->data;
        int delete_next = 0;
        if(cur_fb->end_adress + 1 == next_fb->start_adress){
            cur_fb->end_adress = next_fb->end_adress;
            delete_next = 1;
        }else if(next_fb->end_adress + 1 == cur_fb->start_adress){
            next_fb->end_adress = cur_fb->end_adress;
            current->data = (void *) next_fb;

            next_fb = cur_fb;
            delete_next = 1;
        }

        if(delete_next){
            struct lnk_node * next = current->next;
            __unlink_procedure_of_current(&ps->free_blocks, current, next);
            free(next_fb);
            free(next);
        }
        else{
            current = current->next;
        }
    }
    // TODO: Esta funcion es imprescindible ya que servira para desfragmentar la lista de bloques libres

    return 1;
}

uint32_t mmu_resolve(struct mmu * p_mmu, uint32_t PTBR, uint32_t virt_adress){
    struct pte * pte_match;
    uint32_t frame_adress;
    if (tlb_get_match(p_mmu, &pte_match, PTBR, virt_adress)){
        frame_adress = pte_match->physycal_adress;
    }else{
        mmu_resolve_frame_rootadr_from_memo(p_mmu, &frame_adress, PTBR, virt_adress);
        tlb_add_entry(p_mmu,PTBR,virt_adress,frame_adress);
    }
  

    return frame_adress & ~(FRAME_OFSET_MASK) | (virt_adress & FRAME_OFSET_MASK);
}

int  mmu_resolve_frame_rootadr_from_memo(struct mmu * p_mmu, uint32_t * frame_adress, uint32_t PTBR, uint32_t virt_adress){
    uint32_t selected_page_number = virt_adress & ~(FRAME_OFSET_MASK);
    selected_page_number = selected_page_number >> FRAME_OFSET_BITS;
    
    if(pm_read_word(p_mmu->ps, PTBR) < selected_page_number){
        err(1, "Ilegal virtual memory acccess. Page number exceeded for current PTBR");
        return 0;
    }

    uint32_t pte_adress = PTBR + PAGETABLE_CORE_ADRESS_KOP + selected_page_number * PAGETABLE_ENTRY_ADRESS_KOP;
    *frame_adress = pm_read_word(p_mmu->ps, pte_adress) & ~(FRAME_OFSET_MASK);

    return 1;
}

int tlb_get_match(struct mmu * p_mmu, struct pte ** match_pte, uint32_t ptbr, uint32_t virt_adress){
    struct pte * potential_pte;

    uint32_t page_number = virt_adress >> FRAME_OFSET_BITS;
    int hash = page_number % p_mmu->tlb_max_space;
    potential_pte  = &p_mmu->tlb_hashArr[hash];
    * match_pte = NULL;
    if(potential_pte->active && (potential_pte->virtual_adress & ~(FRAME_OFSET_MASK)) == (virt_adress & ~(FRAME_OFSET_MASK))){
        * match_pte = potential_pte;
        return 1;
    }else{
        return 0;
    }
}

int tlb_add_entry(struct mmu * p_mmu, uint32_t ptbr, uint32_t virt_adr, uint32_t phys_adr){
    uint32_t page_number = virt_adr >> FRAME_OFSET_BITS;
    int hash = page_number % p_mmu->tlb_max_space;
    struct pte * new_pte = &p_mmu->tlb_hashArr[hash];
    new_pte->active = 1;
    new_pte->physycal_adress = phys_adr & ~(FRAME_OFSET_MASK);
    new_pte->virtual_adress = virt_adr & ~(FRAME_OFSET_MASK);
}

int tlb_flush(struct mmu * p_mmu){
    struct pte * current_pte;
    for(int i = 0; i < p_mmu->tlb_max_space; i++){
        current_pte = &p_mmu->tlb_hashArr[i];
        current_pte->active = 0;
        current_pte->physycal_adress = 0;
        current_pte->virtual_adress = 0;
    } 
}


int pmemo_init(struct physycal_memory * pm){
    pm->memory= (char *) malloc (sizeof(char)*BUS_ADRESS_SPACE);
    lnklst_LFRL_init(&(pm->free_blocks));
    struct free_block * root_freeBlock = ( struct free_block *) malloc(sizeof(struct free_block));
    root_freeBlock->start_adress= ALL_MEMO_SPACE_START_ADRESS;
    root_freeBlock->end_adress= ALL_MEMO_SPACE_END_ADRESS;
    lnklst_LFRL_push(&(pm->free_blocks),(void *) root_freeBlock);
}

int mmu_init(struct mmu * target_mmu, struct physycal_memory * pm, int max_tlb_space){
    target_mmu->tlb_hashArr = (struct pte *) malloc(sizeof(struct pte)*max_tlb_space);
    for(int i = 0; i < target_mmu->tlb_max_space; i++) target_mmu->tlb_hashArr[i].active = 0;
    target_mmu->tlb_max_space = max_tlb_space;
    target_mmu->ps = pm;
}

int mmu_malloc(struct mmu * target_mmu, uint32_t * ptbr, int32_t word_kop){
    int frames_needed = word_kop/FRAME_SIZE;
    if(word_kop % FRAME_SIZE){
        frames_needed++;
    }
    return pm_pt_page_table_malloc(target_mmu->ps, ptbr, frames_needed);
}

int mmu_free(struct mmu * target_mmu, uint32_t * ptbr){
    pm_pt_page_table_free(target_mmu->ps, ptbr);
    
}
