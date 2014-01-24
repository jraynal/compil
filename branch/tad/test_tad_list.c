#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "list.h"

#ifndef DEBUG
#define DB(say)
#else
#define DB(say) fprintf(stderr,#say"\n");
#endif

struct _variable *init_data(const char * val){
	struct _variable *var = (struct _variable*)malloc(sizeof(struct _variable));
	var->type=INT_TYPE;
	var->addr=val;
	return var;
}
void print_var(struct _variable *data){
	if(data!=NULL){
		printf("/%s(%d)",data->addr,(int)data->type);
	}
}

void print_list(struct _list* l){
	if(l == NULL)
		fprintf(stderr, "No list passed in arguments (%s,%d)\n",__FILE__,__LINE__);
	int size = l->size;
	printf("List of %d element%s : \t",size,size>1?"s":"");
	struct _nodeList * node = l->head;
	while(node != NULL){
		print_var((struct _variable *) node->value);
		node = node->next;
	}
	printf("\n");

}


void test_var(struct _variable *data, const char * val){
	if(data!=NULL) {
		DB(Testing returned values)
		assert(strcmp(data->addr,val)==0);
	}
	else
		assert(1==0);
}

void test_init_list(){
	struct _list * list =init_list();
	assert(list != NULL);
	assert(EXIT_SUCCESS==del_list(list));
}

void test_set(){
	struct _list * l = init_list();
	struct _variable* data[10] ;
	data[0] = init_data("42");
	data[1] = init_data("43");
	data[2] = init_data("43");
	data[3] = init_data("7");
	data[4] = init_data("1");
	assert(is_empty(l));
	// print_list(l);

	DB(insert : data1);
	insertElmnt(data[4],l);
	assert(!is_empty(l));
	// print_list(l);
	assert(l->size==1);

	DB(remove : data1);
	removeElmnt(data[4],l);
	// print_list(l);
	assert(is_empty(l));

	assert(EXIT_SUCCESS==del_list(l));
	free(data[4]);

	l = init_list();

	DB(insert : data42);
	insertElmnt(data[0],l);
	assert(!is_empty(l));
	// print_list(l);
	assert(l->size==1);

	DB(insert : data43);
	insertElmnt(data[1],l);
	// print_list(l);
	assert(l->size==2);

	DB(insert : data43);
	insertElmnt(data[2],l);
	// print_list(l);
	assert(l->size==3);

	DB(remove : data7);
	removeElmnt(data[3],l);
	// print_list(l);
	assert(l->size==3);

	DB(remove : data43);
	removeElmnt(data[1],l);
	// print_list(l);
	assert(l->size==2);


	assert(EXIT_SUCCESS==del_list_and_content(l));
	free(data[3]);
	free(data[1]);
}

int main(){
	DB(Debut des tests)
	
	DB(Test initialisation)
	test_init_list();
	DB(Test seter && getter)
	test_set();
	
	return EXIT_SUCCESS;
}
