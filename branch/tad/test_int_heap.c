#include <assert.h>
#include "int_heap.h"

#ifndef DEBUG
#define DB(say)
#else
#define DB(say) fprintf(stderr,#say"\n");
#endif

#define TEST1 "my name is bob\n"
#define TEST2 "my name is michel and i am %d\n",12 


int main() {
	DB(##################################################);
	DB(Test de Int_heap:);
	
	int val;

	DB(Initialisation...)
	struct _int_heap * heap = init_int_heap();
	assert(heap != NULL);
	assert(heap->first_free == 0);

	DB(Get new registers);
	val = get_free_int(heap);
	printf("%d\n",val );
	assert(heap != NULL);
	assert(heap->first_free == 1);
	assert(heap->tree != NULL);
	assert(heap->tree->weight == 0);

	val = get_free_int(heap);
	printf("%d\n",val );
	val = get_free_int(heap);
	printf("%d\n",val );
	val = get_free_int(heap);
	printf("%d\n",val );
	val = get_free_int(heap);
	printf("%d\n",val );
	assert(heap->first_free == 5);
	assert(heap->tree != NULL);
	assert(heap->tree->weight == 0);

	DB(free some registers)
	printf("3\n");
	free_int(heap , 3);
	assert(heap->first_free == 5);
	assert(heap->tree->weight == 1);
	printf("2\n");
	free_int(heap , 2);
	assert(heap->first_free == 5);
	assert(heap->tree->weight == 2);


	DB(Get registers);
	// print_content(heap);
	val = get_free_int(heap);
	printf("%d\n",val );
	assert(val == 3 || val == 2);
	assert(heap->tree->weight == 1);
	// print_content(heap);
	val = get_free_int(heap);
	printf("%d\n",val );
	assert(val == 3 || val == 2);
	assert(heap->tree->weight == 0);
	val = get_free_int(heap);
	printf("%d\n",val );

	DB(Deletion...)
	delete_int_heap(heap);
	DB(##################################################);
	return EXIT_SUCCESS;
}