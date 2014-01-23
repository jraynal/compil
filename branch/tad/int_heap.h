#ifndef INT_HEAP
#define INT_HEAP

#include <stdlib.h>
#include <stdio.h>

#include "structs.h"


struct _int_heap * init_int_heap();

int delete_int_heap(struct _int_heap* h);

int get_free_int(struct _int_heap* h);

int free_int(struct _int_heap* h , int num);

//For debug
void print_content(struct _int_heap *h);


#endif


