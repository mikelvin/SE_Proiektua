#ifndef LNKLIST_FUNCTIONALITY
#define LNKLIST_FUNCTIONALITY

#include <lnklist_s.h>

struct lnk_node* SortedMerge(struct lnk_node* a, struct lnk_node* b, int (*comparator) (void * data_A, void * data_B));
void FrontBackSplit(struct lnk_node* source,
					struct lnk_node** frontRef, struct lnk_node** backRef);
void MergeSort(lnklist_LFRL * list, int (*comparator) (void * data_A, void * data_B));
void __mergeSort(struct lnk_node** headRef, int (*comparator) (void * data_A, void * data_B));

#endif
