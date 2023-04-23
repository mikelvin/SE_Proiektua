#include <loader.h>
#include <string.h>
#include <memory_s.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <executor.h>

#define LOADER_MAXTLB 10

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int __loader_getValidPid(loader_s * loader){
    int new_pr_id;
    new_pr_id = loader->prid_counter++;
    if(loader->prid_counter<0){ 
       loader->prid_counter = 1; 
    }

    return new_pr_id;
}
int __loader_getValidPrio(loader_s * loader){
    return (rand() % (loader->max_prio));
}
/*
    Loader struct-a hasieraten du eta LOAD_DIR en dauden fitxategi guztien helbideak gordetzen ditu, ondoren hauek kargatu ahal izateko.

*/
int loader_startup(loader_s * loader, struct physycal_memory * pm, sched_basic_t * sched, int max_prio){
    loader->prid_counter = 1;
    loader->max_prio = max_prio;
    mmu_init(&loader->mmu, pm, LOADER_MAXTLB);
    loader->sched = sched;
    lnklst_LFRL_init(&loader->unloaded_elf_list);

    DIR *d;
    struct dirent *dir;

    d = opendir(LOAD_DIR);
    if (d)
    {

        int line_numb = 0;
        while ((dir = readdir(d)) != NULL)
        {
            if (!strcmp (dir->d_name, "."))
                continue;
            if (!strcmp (dir->d_name, ".."))    
                continue;

            char * file_full_path = concat(LOAD_DIR, dir->d_name);

            printf("%s", file_full_path);
            
            lnklst_LFRL_push(&loader->unloaded_elf_list, file_full_path);
        }
        closedir(d);
    }
    return(0);
}

int loader_loadNextProcess(loader_s * loader)
{
    char * current_path = (char *) lnklst_LFRL_pop(&loader->unloaded_elf_list);
    if(current_path == NULL){
        return 0;
    }

    struct pcb_str * curr_pcb = (struct pcb_str *) malloc(sizeof(struct pcb_str));
    
    pcb_init(curr_pcb, __loader_getValidPid(loader), __loader_getValidPrio(loader));

    __load_file(&loader->mmu, current_path, curr_pcb);

    sched_AddToProcessQueue(loader->sched, curr_pcb);

    return 1;
}
/**
 * @brief Parametro bitartez jasotako string-ean dagoen PATH eko fitxategia kargatzen du memorian eta parametro
 * bitartez zehaztutako pcb-arekin lotzen du.
 * 
 * @param p_mmu Memoria atzitzeko mmu-a. Era honetan, jada definituta dagoen implementazioa erabiliko da
 * @param d_name Sitring bat, Programaren edukia duen fitxategiaren PATH-a izango duena
 * @param loading_pcb Kargatu nahi den PCB-a
 * @return int 
 */
int __load_file(struct mmu *p_mmu, char *d_name, struct pcb_str *loading_pcb)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    size_t read;

    fp = fopen(d_name, "r");
    if (fp == NULL){
        exit(1);
        return 0;
    }

    // Zenbatu fitxategiaren lerroak: Erreserbatu behar den word kopurua jakiteko
    int32_t line_numb = 0;
    while ((read = getline(&line, &len, fp)) != -1) {line_numb++;}
    fseek (fp, 0 , SEEK_SET );
    if(line_numb < 2){
        return 0;
    }

    mmu_malloc(p_mmu, &loading_pcb->memo_m.pgb, line_numb-2);
    
    // Lehenengo 2 lerroak prozesatu

    for(int i = 0; i < 2; i++){
        //Line 1: .text XXXXXX
        read = getline(&line, &len, fp);  // .text XXXXXX /r/n
        line[strcspn(line, "\r\n")] = 0;  // .text XXXXXX
        line = strpbrk(line, " ")+1;      // XXXXXX

        if(i == 0){
            loading_pcb->memo_m.code = atoi(line) * 4;
            loading_pcb->s.PC=loading_pcb->memo_m.code; // PCB-aren PC-a hasieratu kode helbidera

        }else{
            loading_pcb->memo_m.data = atoi(line) * 4;
        }
    }

    // Geratzen den programaren edukia kargatu
    uint32_t virt_adress = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
         //printf("Retrieved line %d of length %zu:\n",line_numb, read);
        //printf("%s", line);
        uint32_t current_data = 0;
        line[strcspn(line, "\r\n")] = 0;
        if(!__hexString2int32(line, &current_data)){
            return 0;
        };
        pm_write_word(p_mmu->ps, mmu_resolve(p_mmu,loading_pcb->memo_m.pgb,virt_adress), current_data);
        virt_adress+=4;
    }

    fclose(fp);

    return 1;
}

int loadNullPCB(struct mmu * p_mmu, struct pcb_str * loading_pcb){
    
    mmu_malloc(p_mmu, &loading_pcb->memo_m.pgb, 1);
    loading_pcb->memo_m.data = 0;
    loading_pcb->memo_m.code = 0;

    loading_pcb->curr_prio=0;
    loading_pcb->pid=-1;
    cpu_snapshot_init(&loading_pcb->s, 0);

    pm_write_word(p_mmu->ps, mmu_resolve(p_mmu,loading_pcb->memo_m.pgb,0), EXIT_CODE);
}

int __hexString2int32(char * hexString, uint32_t * intVal){
    if(strlen(hexString) > 8){
        return 0;
    }
    
    char * curr_char = hexString;
    uint32_t ema = 0;
    uint8_t curr_int_val;
    while(*curr_char != '\0'){
        if(!__hexChar2int4(*curr_char, &curr_int_val)){
            return 0;
        };

        ema = ema << 4; 
        curr_int_val = curr_int_val & (0b1111); // Aplikatu maskara emaitza egokia zihurtatzeko.
        ema = ema | curr_int_val;
        curr_char++;
    }

    *intVal = ema;
    return 1;

}

int __hexChar2int4(char hexChar , uint8_t * intVal){
    
    if('0' <= hexChar && hexChar <= '9'){
        *intVal =  hexChar - '0';
    }else if ('A' <= hexChar && hexChar <= 'F'){
        *intVal =  hexChar - 'A' + 10;
    }else if ('a' <= hexChar && hexChar <= 'f'){
        *intVal =  hexChar - 'a' + 10;
    }else{
        return 0; // Error
    }
    return 1;

}

/* int main(){
    lnklist_LFRL lnk;
    lnklst_LFRL_init(&lnk);
    //loader(&lnk);
} */