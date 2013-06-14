#include "list.h"

typedef int (List_compare)(void *a, void *b);

int List_bubble_sort(List *list, List_compare *comp);

List *List_merge_sort(List *list, List_compare *comp);
