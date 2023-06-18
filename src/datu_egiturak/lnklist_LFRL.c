// PCB:
#include <stdlib.h>
#include <stdio.h>
#include <lnklist_s.h>
#include <stdbool.h>
#include <assert.h>

int __unlink_procedure_of_current(lnklist_LFRL * p_list, struct lnk_node * prev, struct lnk_node * current);
int __search_procedure(lnklist_LFRL * p_list,struct lnk_node ** p_prev, struct lnk_node ** p_current, int (* predicate) (void * data, void * args), void * s_args);

/**
 * @brief Elementu bat listaren amaieratik atera eta bueltatzen du. Lista unitate batez txikituko da. 
 * Elementu hau, listaren elementurik nagusiena izango da, hau da, listan gehitu denetik denbora
 * gehiena eman duen elementuarena.  
 * 
 * @param p_list lista
 * @return void* Listatik kendu den elementua
 */
void *lnklst_LFRL_pop(lnklist_LFRL * p_list){
    if (!p_list){
        // Listarik pasatzen ez bada
        return NULL; 
    }
    if(!p_list->first){
        // Lista hutsa bada
        return NULL;
    }

    // Gorde borratu nahi den nodoa: free() egiteko
    struct lnk_node * tmp =  p_list->first;

    // Gorde gure datua
    void * last_item = tmp->data;

    //Eguneratu 
    __unlink_procedure_of_current(p_list, NULL, tmp);

    free(tmp);

    return last_item;
}
/**
 * @brief Listaren azken elementua bueltatzen du hau listatik atera gabe
 * 
 * @param p_list lista
 * @return void* Azken elementua. Azken elementurik ez egotekotan, NULL
 */
void * lnklst_LFRL_peek(lnklist_LFRL * p_list){
    if(!_lnklst_LFRL_notEmpty(p_list)){
        return NULL;
    }

    return p_list->first->data;
}

int lnklst_LFRL_len(lnklist_LFRL * p_list){
    return p_list->len;
}

/**
 * @brief Lista hutsik badagoen adierazi
 * 
 * @param p_list lista
 * @return int : 1 hutsa bada eta 0 elementu bat edo bat baino gehiago baldin badu
 */
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

/**
 * @brief Linked-listaren amaieran, elementu bat gehitzen du.
 * 
 * @param p_list listaren struct-a 
 * @param p_data listara gehitu nahi den datua
 */
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

/**
 * @brief Linked list hutsa hasieratzen du.
 * 
 * @param p_list Hasieratu nahi den lista
 */
void lnklst_LFRL_init(lnklist_LFRL * p_list){
    p_list->first = NULL;
    p_list->last = NULL;
    p_list->len = 0;
}

/**
 * @brief Funtzio pribatua. Current nodoa listatik kanporatzen du, listaren egitura mantenduz. Horretarako prev 
 * nodoa ere behar da. Prev nodoa, current nodoaren aurrekoa izan behar da.
 * 
 * @param[in] p_list lista
 * @param[in] prev current nodoaren aurreko nodoa. Current = first bada, orduan prev NULL izan beharko da
 * @param[in] current listatik kendu nahi den nodoa
 * @return int 
 */
int __unlink_procedure_of_current(lnklist_LFRL * p_list, struct lnk_node * prev, struct lnk_node * current){
    if(!current)
        return 0;
    if(prev)
        assert(prev->next == current);

    if(current == p_list->first){
        assert(!prev);
        p_list->first = p_list->first->next; 
    }else{
        assert(prev);
        prev->next = current->next;
    }

    if(current == p_list->last){
        p_list->last = prev;  
    }
    p_list->len--;

    return 1;
}

/**
 * @brief Funcion privada. Busca en una linked list el primer nodo (current) y su antecesor (prev), que al evaluar el current sobre la funcion "predicate" devuelva 1.
 *  La funcion predicate se evaluara aportando el nodo current y un argumeto de opciones que dependera de la funcion predicate
 * 
 * @param[in] p_list lista
 * @param[out] p_prev nodo previo a p_current
 * @param[out] p_current nodo que aplicando el predicate sobre él, da TRUE
 * @param[in] predicate puntero a funcion
 * @param[in] s_args argumentos de funcion predicate
 * @return int 
 */
int __search_procedure(lnklist_LFRL * p_list,struct lnk_node ** p_prev, struct lnk_node ** p_current, int (* predicate) (void * data, void * args), void * s_args){
    struct lnk_node * prev = NULL;
    struct lnk_node * current = p_list->first;
    int match_found = 0;
    while(current && !match_found){ // Mientras que no se termine la lista y no se hayan producido matches
        if(!(match_found = predicate(current->data, s_args))){ // Comprobar match
            if (prev == NULL /* <=> current == p_list->first*/)               // Si no hay match y el current apunta al inicio de la lista
                prev = p_list->first;       // Hacer que el previo apunte al primero 
            else
                prev = prev->next;          // Sino, adelantar el previo

            current = current->next;
        }
    }
    *p_prev = prev;
    *p_current = current;

    return match_found;
}

/**
 * @brief predicate funtzioarekin bat egiten duen lehenengo datua listatik kendu eta bueltatzen du. Bilaketa listaren atzekaldetik burutuko du. 
 * Hau da, txertatu berriko elementuek lehentasuna izango dute.
 * 
 * @param[in] p_list lista
 * @param[in] predicate listako datu bakoitzarekin ebaluatuko den funtzio boolearra
 * @param[in] s_args predicate funtzioarekin bat gehituko diren argumentuak
 * @return void* 
 */
void * lnklst_LFRL_popFirstMatchFromRear(lnklist_LFRL * p_list, int  (* predicate)    (void * data, void * args), void * s_args){
    if(!p_list || !p_list->len){
        return NULL;
    }

    // FIND DATA
    struct lnk_node * prev ;
    struct lnk_node * current ;
    int match_found;
    match_found = __search_procedure(p_list, &prev, &current, predicate, s_args);

    // POP DATA
    void * out_data = NULL;
    if(match_found){
        __unlink_procedure_of_current(p_list, prev, current);
        out_data = current->data;
        free(current);
    }
    
    return out_data;
}

/**
 * @brief predicate funtzioarekin bat egiten duen lehenengo datua ikuskatzen eta bueltatzen du, hau listatik atera gebe. Bilaketa listaren atzekaldetik burutuko du. 
 * Hau da, txertatu berriko elementuek lehentasuna izango dute.
 * 
 * @param p_list lista
 * @param[in] predicate listako datu bakoitzarekin ebaluatuko den funtzio boolearra
 * @param[in] s_args predicate funtzioarekin bat gehituko diren argumentuak
 * @return void* 
 */
void * lnklst_LFRL_peekFirstMatchFromRear(lnklist_LFRL * p_list, int  (* predicate)  (void * data, void * args), void * s_args){
    if(!p_list || !p_list->len){
        return NULL;
    }

    // FIND DATA
    struct lnk_node * prev ;
    struct lnk_node * current ;
    int match_found;
    match_found = __search_procedure(p_list, &prev, &current, predicate, s_args);

    // PEEK DATA
    void * out_data = NULL;
    if(match_found){
        out_data = current->data;
    }
    
    return out_data;
}

/**
 * @brief Lista errekorritzen du, datu bakoitzaren gainean consumer funtzioa aplikatuz
 * 
 * @param p_list 
 * @param consumer datu bakoitzaren gainean exekutatuko den funtzioa
 * @param args consumer funtzioaren aegumentuak
 * @return void
 */
void lnklst_LFRL_nodeIterator(lnklist_LFRL *p_list, void (*consumer)(struct lnk_node *data, void *s_args), void *args)
{
    struct lnk_node * current = p_list->first;
    int match_found = 0;
    while(current){ // Mientras que no se termine la lista y no se hayan producido matches
        consumer(current, args);
        current = current->next;
    }
}


