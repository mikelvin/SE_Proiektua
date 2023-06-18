
#ifndef MEMORIA_H
#define MEMORIA_H

// define BUS_BIT_SIZE 24
# define BUS_BIT_SIZE 24
# define BUS_ADRESS_SPACE (1<<BUS_BIT_SIZE)

# define TLB_SIZE 64
# define FRAME_OFSET_BITS 8
# define PAGE_BITS (BUS_BIT_SIZE-FRAME_OFSET_BITS)
# define PAGE_KOP (1 << PAGE_BITS)
# define BUS_DATA_BYTES_SIZE 4
# define BUS_DATA_BITS_SIZE BUS_DATA_BYTES_SIZE*8


#define FRAME_SIZE (1<<FRAME_OFSET_BITS)
#define FRAME_OFSET_MASK (FRAME_SIZE-1)
#define FRAME_ADRESS_MASK ((BUS_ADRESS_SPACE-1) & ~(FRAME_OFSET_MASK))

#define PAGETABLE_CORE_ADRESS_KOP 4
#define PAGETABLE_ENTRY_ADRESS_KOP 4

#define DATABUS_CELL_SIZE 4
#define DATABUS_WORD_OFSET_MASK (BUS_DATA_BYTES_SIZE-1)
#define DATABUS_WORD_ADRESS_MASK (BUS_ADRESS_SPACE-1) & ~(BUS_DATA_BYTES_SIZE-1)


// Por cada posicion de memoria --> Cuantos bits se guardan
# define DATA_CELL_STORE_BYTES 1
# define DATA_CELL_STORE_BITS  DATA_CELL_STORE_BYTES*8



# define KERNEL_SPACE_START_ADDRESS 0x000000
# define KERNEL_SPACE_END_ADDRESS 0x3FFFFF
# define MEMO_SPACE_START_ADDRESS 0x400000
# define MEMO_SPACE_END_ADDRESS 0xFFFFFF

# define ALL_MEMO_SPACE_START_ADRESS KERNEL_SPACE_START_ADDRESS
# define ALL_MEMO_SPACE_END_ADRESS MEMO_SPACE_END_ADDRESS


# include <stdint.h>
# include <lnklist_s.h>

struct physycal_memory {
    char * memory;
    lnklist_LFRL free_blocks;
};

struct free_block{
    uint32_t start_adress;
    uint32_t end_adress;
};

struct pte{ // Page Table Entry
    uint32_t virtual_adress;
    uint32_t physycal_adress;
    uint8_t active;
};

struct mmu{
    struct pte * tlb_hashArr;
    int tlb_max_space;
    struct physycal_memory * ps;
};


struct free_block_search_args{
    uint32_t start_limit_adress; // INCLUSIVE
    uint32_t end_limit_adress; // INCLUSIVE
    uint32_t min_adress_kont; 
};

int pmemo_init(struct physycal_memory * pm);
int mmu_init(struct mmu * target_mmu, struct physycal_memory * pm, int max_tlb_space);

int __pm_memory_allocation_procedure(struct physycal_memory * ps, struct free_block_search_args * request, uint32_t * ret_adress);
int __search_space4free(void * free_block, void * search_args);
int __optimize_free_block_list(struct physycal_memory * ps);

int pm_memory_allocation_request(struct physycal_memory * ps, struct free_block_search_args * request, uint32_t * ret_adress);

/** @brief Memorian frame bat erreserbatzen du, memoria nahikoa egotekotan. 
 * Erreserbatutako frame-aren helbidea itzultzen du 
 * 
 * @param[in] ps Memoria fisikoa kudeatzen duen oinarrizko egiturara punteroa
 * @param[out] adress Erreserbatutako frame-aren helbide fisikoa
 * 
 * @returns Arrakasta egoera
 * @retval \c 0 Errorea izatekotan 
 * @retval \c 1 Arrakasta izatekotan
*/
int pm_m_frame_malloc(struct physycal_memory * ps, uint32_t * adress);

/** @brief Aurretik memorian erreserbatu den frame bat askatzen du.
 * 
 * @param[in] ps Memoria fisikoa kudeatzen duen oinarrizko egiturara punteroa
 * @param[in] phys_adress Askatu nahi den frame-aren helbide fisikoa
 * @returns Arrakasta egoera
 * @retval \c 0 Errorea izatekotan 
 * @retval \c 1 Arrakasta izatekotan
*/
int pm_m_frame_free(struct physycal_memory * ps, uint32_t phys_adress);

uint32_t pm_read_word(struct physycal_memory * ps, uint32_t phys_adress);
int pm_write_word(struct physycal_memory * ps, uint32_t phys_adress, uint32_t word);

int pm_pt_page_table_malloc(struct physycal_memory * ps, uint32_t * adress, uint32_t entries);
int pm_pt_page_table_free(struct physycal_memory * ps, uint32_t adress);


// Resolve a virtual adress using MMU: TLB (if match) ifnot, access memory
uint32_t mmu_resolve(struct mmu * p_mmu, uint32_t PTBR, uint32_t virt_adress);

/*
    This method uses specific MMU to 
*/
int  mmu_resolve_frame_rootadr_from_memo(struct mmu * p_mmu, uint32_t * frame_adress, uint32_t PTBR, uint32_t virt_adress);
// 

int mmu_malloc(struct mmu * target_mmu, uint32_t * ptbr, int32_t word_kop);
int mmu_free(struct mmu * target_mmu, uint32_t ptbr);
int mmu_init(struct mmu * target_mmu, struct physycal_memory * pm, int max_tlb_space);


int tlb_get_match(struct mmu * p_mmu, struct pte ** match_pte, uint32_t ptbr, uint32_t virt_adress);
int tlb_add_entry(struct mmu * p_mmu, uint32_t ptbr, uint32_t virt_adr, uint32_t phys_adr);
int tlb_flush(struct mmu * p_mmu);

/*
    KERNEL SPACE:
    - PAGE_TABLE x n (4 + 4 x len)
        - LEN = frame entry kop: uint (4 bytes)
        - PTE (4bytes) x LEN
            * phisical memory adress of frame: 

    MEMORY SPACE:
    - FRAME: (256 size: FF FF 00)
*/

#endif