
//https://www.geeksforgeeks.org/merge-sort-for-linked-list/

// C code for linked list merged sort
#include <stdio.h>
#include <stdlib.h>
#include <lnklist_s.h>
#include <lnklist_functionalities_s.h>

/* function prototypes */


/* sorts the linked list by changing next pointers (not data) */

void MergeSort(lnklist_LFRL * list, int (*comparator) (void * data_A, void * data_B)){
	__mergeSort(&list->first,comparator);
	struct lnk_node * new_last = list->first;
	for(;new_last && new_last->next; new_last = new_last->next){}
	list->last = new_last;
}

void __mergeSort(struct lnk_node** headRef, int (*comparator) (void * data_A, void * data_B))
{
	struct lnk_node* head = *headRef;
	struct lnk_node* a;
	struct lnk_node* b;

	/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->next == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	FrontBackSplit(head, &a, &b);

	/* Recursively sort the sublists */
	__mergeSort(&a, comparator);
	__mergeSort(&b, comparator);

	/* answer = merge the two sorted lists together */
	*headRef = SortedMerge(a, b, comparator);
}

/* See https:// www.geeksforgeeks.org/?p=3622 for details of this
function */
struct lnk_node* SortedMerge(struct lnk_node* a, struct lnk_node* b, int (*comparator) (void * data_A, void * data_B))
{
	struct lnk_node* result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (comparator(a->data, b->data) <= 0) {
		result = a;
		result->next = SortedMerge(a->next, b, comparator);
	}
	else {
		result = b;
		result->next = SortedMerge(a, b->next, comparator);
	}
	return (result);
}

/* UTILITY FUNCTIONS */
/* Split the nodes of the given list into front and back halves,
	and return the two lists using the reference parameters.
	If the length is odd, the extra node should go in the front list.
	Uses the fast/slow pointer strategy. */
void FrontBackSplit(struct lnk_node* source,
					struct lnk_node** frontRef, struct lnk_node** backRef)
{
	struct lnk_node* fast;
	struct lnk_node* slow;
	slow = source;
	fast = source->next;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->next;
		if (fast != NULL) {
			slow = slow->next;
			fast = fast->next;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->next;
	slow->next = NULL;
}

/*
int __coparator_int_test(void * data_A, void * data_B){
	return *((int *) data_B) - *((int *) data_A);
} 
*/
/* Driver program to test above functions*/
// int main()
// {
//     /* Start with the empty list */
//     struct lnklist_fl list;
// 	lnklst_LFRL_init(&list);
	

 
//     /* Let us create a unsorted linked lists to test the functions
// Created lists shall be a: 2->3->20->5->10->15 */
// 	int arr_len = 6;
// 	int arra[] = {15,10,5,20,3,2};
// 	for(int i = 0; i < 6; i++){
// 		printf("%d ", *(int *) arra + i);
// 		lnklst_LFRL_push(&list, arra + i);
// 	}
 
//     /* Sort the above created Linked List */
//     MergeSort(&list, __coparator_int_test);
 
//     printf("Sorted Linked List is: \n");
//     for(struct lnk_node * nod = list.first; nod != NULL; nod = nod->next ){
// 		printf("%d ", *(int *) nod->data);
// 	}
//     return 0;
// }

