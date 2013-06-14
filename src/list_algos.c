#include "list_algos.h"

int List_bubble_sort(List *list, List_compare *comp) {

	// keep on traversing the list until it is sorted
	int is_sorted;
	do{
		// list is sorted by default until disproven
		is_sorted = 1;

		LIST_FOREACH(list, first, next, cur) {
			// if we have reached the last element of the list, break
			if(cur->next == NULL) break;

			// check if two consecutive terms are in the right order
			if(comp(cur->value, cur->next->value) < 0){
				// mark list as not (fully) sorted
				is_sorted = 0;

				// swap the two
				ListNode *one = cur;
				ListNode *two = cur->next;

				// swap outer links
				two->prev = one->prev;
				one->next = two->next;

				// swap inner links
				two->next = one;
				one->prev = two;
			}
		}
	}while(!is_sorted);

	return 1;

//TODO: Implement error checking
error:
	return 0;
}

void *List_merge_sort(List *list, List_compare *comp) {

	// you have a list; obtain two sorted sub-lists
	List *upper;
	List *lower;

	// TODO: Implement list_split
	List_split(list, upper, lower);

	List *left = List_merge(upper, comp);
	List *right = List_merge(lower, comp);

	// you have two sorted sub-lists; merge them!
	void *a;
	void *b;

	// allocate and return sorted List
	List *out = List_create();

	while(1) {
		// update the condidate
		a = List_pop(List *left);
		b = List_pop(List *right);

		// check if one of the lists is exhausted
		if(a == NULL) {
			// append all elements from the other list
			while((b = List_pop(right)) != NULL) {
				List_push(out, b);
			}
			break;

		} else if(b == NULL) {
			// append all elements from the other list
			while((a = List_pop(left)) != NULL) {
				List_push(out, a);
			}
			break;

		} else {
			// append the smaller candidate to the list
			if(comp(a,b) < 0) {
				List_push(out, a);
			} else {
				List_push(out, b);
			}
		}
	};
	return out;

// TODO: Implement error checking
error:
	return NULL;
}











