#include "darray_algos.h"
#include <stdlib.h>

#ifndef _quicksort_INITIAL_MAX
#define _quicksort_INITIAL_MAX 5
#endif

int DArray_qsort(DArray *array, DArray_compare cmp) {
	qsort(array->contents, DArray_count(array), sizeof(void *), cmp);
	return 0;
}

int DArray_heapsort(DArray *array, DArray_compare cmp) {
	return heapsort(array->contents, DArray_count(array), sizeof(void *), cmp);
}

int DArray_mergesort(DArray *array, DArray_compare cmp) {
	return mergesort(array->contents, DArray_count(array), sizeof(void *), cmp);
}

/* My own implemenatation of quicksort */
int DArray_quicksort(DArray *array, DArray_compare cmp) {
	debug( ">>> DArray_quicksort\n");
	check(array != NULL, "array is NULL\n");
	debug( "array: %p\n", array);

	// declare here for error handling
	DArray *less;
	DArray *more;

	if(DArray_count(array) <= 1) {
	// base-case: array is empty or singleton
		debug( "array less/equal 1\n");
		return 0;

	} else {
		// recursive-case: array is longer than 1
		debug( "array greater than 1\n");
		
		// pick a pivot
		void *pivot = DArray_pop(array);
		check(pivot != NULL, "pivot is NULL");
		debug("length of array: %d", DArray_count(array));	

		// make a list of larger and smaller elements
		less = DArray_create(sizeof(void *), _quicksort_INITIAL_MAX);
		more = DArray_create(sizeof(void *), _quicksort_INITIAL_MAX);
		debug("create less and more");
		void *cur;
		int i;

		for(i = 0, cur = DArray_first(array); 
			i < DArray_count(array);
			i++, cur = DArray_get(array, i)) {

			check(cur != NULL, "cur is NULL");

			debug("comparing cur to pivot");
			debug("cur: %s", (char *)cur);
			debug("pivot: %s", (char *)pivot);
			
			debug("address of comparator: %p", cmp);
			debug("cmp(&pivot, &cur): %d", cmp(&pivot, &cur));

			if(cmp(&pivot, &cur) < 0){
				debug("cur is less than the pivot");
				DArray_push(less, cur);
			}else{
				debug("cur is more than the pivot");
				DArray_push(more, cur);
			}

		}

		// get lengths before
		int len_less_before = DArray_count(less);
		int len_more_before = DArray_count(more);

		// sort the sublists
		int rc1 = DArray_quicksort(less, cmp);
		int rc2 = DArray_quicksort(more, cmp);
		check(rc1 == 0, "failed to sort less sublist");
		check(rc2 == 0, "failed to sort more sublist");

		// check lengths
		check(len_less_before == DArray_count(less), "length of less changes");
		check(len_more_before == DArray_count(more), "length of more changes");
		
		// at this point less and more are already sorted

		// write all elements back into the old array
		int j;
		for(j = 0, i = 0, cur = DArray_first(less); 
			i < DArray_max(less); 
			j++, i++, cur = DArray_get(less, i)) {

			DArray_set(array, j, cur);
		}

		DArray_set(array, ++j, pivot);

		for(i = 0, cur = DArray_first(more); 
			i < DArray_max(more); 
			j++, i++, cur = DArray_get(more, i)) {

			DArray_set(array, j, cur);
		}

		// check size
		int s_array = DArray_count(array);
		int s_less = DArray_count(less);
		int s_more = DArray_count(more);
		check(s_array = (s_less + s_more + 1), "number of elements changes");

		// destroy the temporary arrays
		DArray_destroy(less);
		DArray_destroy(more);

		return 0;
	}

error:
// free all allocated memory
if(array) DArray_destroy(array);
if(less) DArray_destroy(less);
if(more) DArray_destroy(more);

return 1;
}
