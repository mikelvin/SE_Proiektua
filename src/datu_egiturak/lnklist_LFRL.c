// PCB:
#include <stdlib.h>
#include <stdio.h>
#include <lnklist_s.h>
#include <stdbool.h>


void *lnklst_LFRL_pop(lnklist_LFRL * p_list){
    if (!p_list){
        // Listarik pasatzen ez bada
        return NULL; 
    }
    if(!p_list->first){
        // Lista hutsa bada
        return NULL;
    }

    // Gorde gure datua
    void * last_item = p_list->first->data;

    // Gorde borratu nahi den nodoa: free() egiteko
    struct lnk_node * tmp =  p_list->first;


    //Eguneratu 
    p_list->first = p_list->first->next;

    if(p_list->first == NULL){
        p_list->last = NULL;
    }
    p_list->len--;

    free(tmp);

    return last_item;
}

void * lnklst_LFRL_peek(lnklist_LFRL * p_list){
    if(!_lnklst_LFRL_notEmpty(p_list)){
        return NULL;
    }

    return p_list->first->data;
}

int lnklst_LFRL_len(lnklist_LFRL * p_list){
    return p_list->len;
}

int _lnklst_LFRL_notEmpty(lnklist_LFRL * p_list){
    if (!p_list){
        // Listarik pasatzen ez bada
        return false; 
    }
    if(p_list->len == 0){
        return true;
    }else{
        return false;
    }
}

void lnklst_LFRL_push(lnklist_LFRL * p_list, void * p_data){
    if (!p_list){
    // Listarik pasatzen ez bada
        return; 
    }

    // Memoria erreserbatu
    struct lnk_node * n_berri;
    n_berri=(struct lnk_node *)malloc(sizeof(struct lnk_node));

    if(n_berri==NULL){
            printf("nOut of Memory Space:n");
            exit(0);
    }

    n_berri->data = p_data;
    n_berri->next = NULL;
    
    if(!p_list->last){
        // Lista hutsa bada
        p_list->first = n_berri;
    }else{
        p_list->last->next=n_berri;
    }

    p_list->last = n_berri;
    p_list->len++;
}



void lnklst_LFRL_init(lnklist_LFRL * p_list){
    p_list->first = NULL;
    p_list->last = NULL;
    p_list->len = 0;
}
