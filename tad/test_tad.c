#include <stdio.h>
#include <assert.h>
#include "tree.h"

#ifndef DEBUG
#define DB(say)
#else
#define DB(say) fprintf(stderr,#say"\n");
#endif

struct _variable *init_data(int val){
	struct _variable *var = (struct _variable*)malloc(sizeof(struct _variable));
	var->type=INT_TYPE;
	var->value.ival=val;
	return var;
}
void print_var(struct _variable *data){
	if(data!=NULL){
		printf("type: %d, value\t%d\n",data->type,data->value.ival);
	}
}

void test_init_tree(){
	struct _node *tree = init_tree();
	assert(tree!=NULL);
	assert(EXIT_SUCCESS==del_tree(tree));
}

void test_set(){
	struct _node *root = init_tree();
	char *b="/b";
	char *bo="/bo";
	char *bob="/bob";
	DB(set node)
	set_node(root,bob,init_data(42));
	DB(get node without value)
	assert(get_node(root,b)==NULL);
	DB(twice)
	assert(get_node(root,bo)==NULL);
	DB(get good variable)
	assert(get_node(root,bob)!=NULL);
	print_var(get_node(root,bob));
	set_node(root,bo,init_data(23));
	print_var(get_node(root,bo));
	print_var(get_node(root,bob));
	set_node(root,bob,init_data(12));
	print_var(get_node(root,bo));
	print_var(get_node(root,bob));
	DB(free)
	assert(EXIT_SUCCESS==del_tree(root));
}

int main(){
	DB(Debut des tests)
	
	DB(Test initialisation)
	test_init_tree();
	DB(Test seter && getter)
	test_set();
	
	return EXIT_SUCCESS;
}
