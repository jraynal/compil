#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tree.h"

#ifndef DEBUG
#define DB(say)
#else
#define DB(say) fprintf(stderr,#say"\n");
#endif

struct _variable *init_data(const char *  val){
	struct _variable *var = (struct _variable*)malloc(sizeof(struct _variable));
	var->type=INT_TYPE;
	var->addr=val;
	return var;
}
void print_var(struct _variable *data){
	if(data!=NULL){
		printf("type: %d, addr \t%s\n",data->type,data->addr);
	}
}

void test_var(struct _variable *data, const char * val){
	if(data!=NULL) {
		DB(Testing returned values)
		assert(strcmp(data->addr,val)==0);
	}
	else
		assert(1==0);
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
	char *victoria="/victoria";
	char *vob="/vob";
	char *accel="/$accel";
	DB(set node)
	set_node(root,accel,init_data("111"));
	test_var(get_node(root,accel),"111");
	set_node(root,bob,init_data("42"));
	DB(get node without value)
	assert(get_node(root,b)==NULL);
	DB(twice)
	assert(get_node(root,bo)==NULL);
	DB(get good variable)
	assert(get_node(root,bob)!=NULL);
	test_var(get_node(root,bob),"42");
	
	set_node(root,bo,init_data("23"));
	test_var(get_node(root,bo),"23");
	test_var(get_node(root,bob),"42");
	
	set_node(root,bob,init_data("12"));
	test_var(get_node(root,bo),"23");
	test_var(get_node(root,bob),"12");
	
	set_node(root,victoria,init_data("1"));
	test_var(get_node(root,victoria),"1");
	assert(get_node(root,vob)==NULL);
	set_node(root,vob,init_data("123"));
	test_var(get_node(root,vob),"123");
	test_var(get_node(root,bob),"12");
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
