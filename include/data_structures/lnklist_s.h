#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct lnklist_fl {
    struct lnk_node *first; // Listaren hasiera da. TAIL --> NODE->NODE->...
    struct lnk_node *last; // Listaren amaiera da. HEAD --> NODE->|| 
    int len;
};

struct lnk_node {
    void  * data;
    struct lnk_node *next;
};

/** 
 * Linked pr_list_LFRL datu egitura: pr_lnklist_fl -en oinarritu
 *  - LFRL:
 *      - Linked
 *      - Reversed
 *      - First Last
 * 
 * KOSTUAK:
 *  - PUSH: O(1) : pr_lst_LFRL_push
 *  - POP: O(1) : pr_lst_LFRL_pop
 *  - INSERT: O(N)
 *  - REMOVE: O(N)
 *  - ITER: 
 *  -   - Correcto: O(2N)
 *  -   - Reverso: O(N)
*/
typedef struct lnklist_fl lnklist_LFRL;

/**
 * pcb_t motako elementurik nagusiena (old) kanporatzen du pr_lnklist_fl motako p_list-atik. Hau da, listaren hasieran dagoena.
 * 
 * AURREBALDINTZAK:
 *  - ez dago aurrebaldintzik
 * 
 * POSTBALTDINTZAK:
 *  - pcb_t motako elemetua listaren hasierarik kanporatu.
 *  - p_list == NULL, orduan NULL bueltatuko du
 *  - p_list hutsa bada, orduan NULL bueltatuko du
 * 
 * EZAUGARRIAK:
 * - Exekuzio kostua: O(1)
 * 
 * IMPLEMENTAZIO_OHARRAK:
 *  - none
*/
void *lnklst_LFRL_pop(lnklist_LFRL * p_list);

/**
 * pcb_t motako struct bat emanda, elementu bat gehitzen du pr_lnklist_fl motako p_list-aren amaieran. 
 * 
 * AURREBALDINTZAK:
 *  - ez dago aurrebaldintzik
 * 
 * POSTBALTDINTZAK:
 *  - s_pcb motako p_pcb listaren amaieran gehitu
 *  - p_list == NULL, orduan ez du ezer egingo 
 *  
 * ERROREAK: TODO: Ez dira era egokian tratatzen
 *  - Nodoa gehitzeko memoria nahikorik ez badu --> exit(0) 
 * 
 * EZAUGARRIAK:
 * - Exekuzio kostua: O(1)
 * 
 * IMPLEMENTAZIO_OHARRAK:
 *  - Null p_pcb-k gehitu daitezke
*/
void lnklst_LFRL_push(lnklist_LFRL * p_list, void * p_data);

void * lnklst_LFRL_peek(lnklist_LFRL * p_list);

int lnklst_LFRL_len(lnklist_LFRL * p_list);

int _lnklst_LFRL_notEmpty(lnklist_LFRL * p_list);

void * lnklst_LFRL_popFirstMatchFromRear(lnklist_LFRL * p_list, int (* predicate)    (void * data, void * args), void * s_args);
void * lnklst_LFRL_peekFirstMatchFromRear(lnklist_LFRL * p_list, int  (* predicate)  (void * data, void * args), void * s_args);

void lnklst_LFRL_init(lnklist_LFRL * p_list);

#endif
