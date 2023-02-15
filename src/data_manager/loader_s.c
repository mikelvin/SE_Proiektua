#include <loader.h>
#include <string.h>
#include <memory_s.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int loader(struct mmu * p_mmu, lnklist_LFRL * pcb_list){
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
            struct pcb_str * new_pcb = malloc(sizeof(struct pcb_str));
            load_file(p_mmu, file_full_path, new_pcb);
            
            lnklst_LFRL_push(pcb_list, new_pcb);
        }
        closedir(d);
    }
    return(0);
}
int load_file(struct mmu * p_mmu, char * d_name, struct pcb_str * loading_pcb){
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(d_name, "r");
    if (fp == NULL)
        exit(1);

    // Zenbatu fitxategiaren lerroak: Erreserbatu behar den word kopurua jakiteko
    int32_t line_numb = 0;
    while ((read = getline(&line, &len, fp)) != -1) {line_numb++;}
    fseek (fp, 0 , SEEK_SET );
    if(line_numb < 2){
        return 0;
    }

    mmu_malloc(loading_pcb->memo_m, &loading_pcb->memo_m.pgb, line_numb-2);
    
    for(int i = 0; i < 2; i++){
        //Line 1: .text XXXXXX
        read = getline(&line, &len, fp);  // .text XXXXXX /r/n
        line[strcspn(line, "\r\n")] = 0;  // .text XXXXXX
        line = strpbrk(line, " ")+1;      // XXXXXX

        if(i == 0){
            hexString2int32(line, &loading_pcb->memo_m.code);
        }else{
            hexString2int32(line, &loading_pcb->memo_m.data);
        }
    }

    uint32_t virt_adress = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
         //printf("Retrieved line %d of length %zu:\n",line_numb, read);
        //printf("%s", line);
        uint32_t current_data = 0;
        line[strcspn(line, "\r\n")] = 0;
        if(!hexString2int32(line, &current_data)){
            return 0;
        };
        pm_write_word(p_mmu, mmu_resolve(p_mmu,loading_pcb->memo_m.pgb,virt_adress), current_data);
        virt_adress++;
    }

    fclose(fp);
    if (line)
        free(line);

    return 1;
}

/**
 * @brief 8 karaktereko String batean kodetuta dauden 4 byteko (32bit) hitzaren uint32 balioa jaso
 * 
 * @param hexStringLen8 8 karaktere edo gutxiko String-a. Bertan HEX formatuan 4 byte kodetuta egon beharko dira. 8 karaktere baino gehiago zehazten badira, errorea jasoko da.
 * @param intVal 
 * @return int 
 */
int hexString2int32(const char * hexString, uint32_t * intVal){
    if(strlen(hexString) > 8){
        return 0;
    }
    
    char * curr_char = hexString;
    uint32_t ema = 0;
    uint8_t curr_int_val;
    while(*curr_char != '\0'){
        if(!hexChar2int4(*curr_char, &curr_int_val)){
            return 0;
        };

        ema = ema << 4; 
        curr_int_val = curr_int_val & (0b1111); // Aplikatu maskara emaitza egokia zihurtatzeko.
        ema = ema || curr_int_val;
        curr_char++;
    }

    *intVal = ema;
    return 1;

}

/**
 * @brief ASCII-n kodetuta dagoen Hex karaktere baten integer balio erreala bueltatu.
 * 
 * Hau da da, \c ASCII-n '0' izango zenari, 0 \c int balioa eman. Eta 'F' izango zenari, 16 balioa eman.
 * 
 * @param hexChar ASCII-n kodetuta dagoen HEX karakterea
 * @param intVal HEX balioaren integer balio erreala.
 * @return \c int Success egoera
 */
int hexChar2int4(char hexChar , uint8_t * intVal){
    
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

int main(){
    lnklist_LFRL lnk;
    lnklst_LFRL_init(&lnk);
    //loader(&lnk);
}