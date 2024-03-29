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

		/*
		for(i = 0, cur = DArray_first(array); 
			i < DArray_count(array);
			i++, cur = DArray_get(array, i)) {
		*/

		for(cur = DArray_pop(array); cur != NULL; cur = DArray_pop(array)) {

			check(cur != NULL, "cur is NULL");
			debug("length of array: %d; i: %d", DArray_count(array), i);

			debug("comparing cur to pivot");
			debug("cur: %s", (char *)cur);
			debug("pivot: %s", (char *)pivot);
			
			debug("address of comparator: %p", cmp);
			debug("cmp(&pivot, &cur): %d", cmp(&pivot, &cur));

			if(cmp(&pivot, &cur) > 0){
				debug("cur is less than the pivot");
				DArray_push(less, cur);
			}else{
				debug("cur is more than the pivot");
				DArray_push(more, cur);
			}
		}

		check(DArray_count(array) == 0, "array not empty");

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
		debug("old len(more): %d; new len(more): %d", len_more_before, DArray_count(more));
		check(len_more_before == DArray_count(more), "length of more changes");
		
		// at this point less and more are already sorted

		// write all elements back into the old array
		for( i = 0, cur = DArray_first(less); 
			i < DArray_count(less); 
			i++, cur = DArray_get(less, i)) {

			debug("pushing from less, length of less: %d; i: %d", DArray_count(less), i);
			DArray_push(array, cur);
		}

		DArray_push(array, pivot);
			debug("pushing from pivot");

		for(i = 0, cur = DArray_first(more); 
			i < DArray_count(more); 
			i++, cur = DArray_get(more, i)) {

			debug("pushing from more");
			DArray_push(array, cur);
		}

		// check size
		int s_array = DArray_count(array);
		int s_less = DArray_count(less);
		int s_more = DArray_count(more);
		check(s_array = (s_less + s_more + 1), "number of elements changes");

		// destroy the temporary arrays
		DArray_destroy(less);
		DArray_destroy(more);

		// give feedback
		#ifndef NDEBUG
		int k;
		for(k=0; k<DArray_count(array); k++) {
			debug("element %d: %s", k, DArray_get(array, k));
		}
		#endif

		return 0;
	}

error:
// free all allocated memory
if(array) DArray_destroy(array);
if(less) DArray_destroy(less);
if(more) DArray_destroy(more);

return 1;
}

/* My own implementation of merge sort */
/*
int DArray_mergesort(DArray *array, DArray_compare cmp) {

	// Base-case: List has length 1 or less
	if(DArray_count(array) <= 1) {
		// do nothing to the array; just bottom out
		return 1;
	}

	// Recursive case: list has length greater than 1
	} else {
		// split the array into two sub-arrays and sort them
		DArray *left = DArray_create(_quicksort_INITIAL_MAX);
		DArray *right = DArray_create(_quicksort_INITIAL_MAX);
		check_mem(left);
		check_mem(right);

		void *cur;
		int par;
		for(cur = DArray_pop(array), par = 0;
			cur != NULL;
			cur = DArray_pop(array), par = ((par + 1) % 2);
			check((par == 0) || (par == 1), "parity flag off: %d", par)) {

			if(par == 0) {
				DArray_push(left, cur);
			} else {
				DArray_push(right, cur);
			}
		}
		// check that the old array is now empty
		check(DArray_count(array) == 0, "old array not empty: %d", DArray_count(array));
		// sort sub-lists
		int rc1 = DArray_mergesort(DArray left, cmp);
		int rc2 = DArray_mergesort(DArray right, cmp);

		// merge sub-lists
		void *L = DArray_pop(left);
		void *R = DArray_pop(right);

		while((L != NULL) && (R != NULL)){

			if(cmp(L,R) < 0) {
				DArray_push(array, L);
				L = DArray_pop(left);
			} else {
				DArray_push(array, R);
				R = DArray_pop(right);
			}
		}
		check(!((L != NULL) && (R != NULL)), "Neither sub-list is empty");

		// create alias for non-empty sub-list
		DArray *rem;
		void *V;

		if(L != NULL) {
			rem = left;
		
		} else if(R != NULL) {
			rem = right;

		} else {
			rem = NULL;
		}
	
		if(rem) {
			for(V = DArray_pop(rem);
					V != NULL;
					V = DArray_pop(rem)) {

				DArray_push(array, V);	
			}
		}



	}



}
*/
