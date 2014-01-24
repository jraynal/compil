#include "layer.h"


#define CHK(truc) do{if(truc == NULL){fprintf(stderr,#truc" is NULL by %s in %s line %d\n",__FUNCTION__,__FILE__,__LINE__); exit(1);}}while(0)

static int alphonse=0;
//create a new context
struct _layer * init_layer(){
	struct _layer * layer = malloc(sizeof(struct _layer));
	CHK(layer);
	layer-> root = init_tree();
	layer-> previous = NULL;
	fprintf(stderr,"Layer %d initialized\n",++alphonse);
	return layer;
}


// add a new current context, the previous one will be the father
struct _layer * add_layer(struct _layer * layer){
	CHK(layer);
	struct _layer * son = init_layer();
	CHK(son);
	son-> previous = layer;
	fprintf(stderr,"Layer %d added\n",alphonse);
	return son;
}


// get the variable from the current context (or above if it does not exist)
struct _variable * get_var_layer(struct _layer * layer , char * name){
	CHK(layer);
	CHK(name);
	struct _variable * var = NULL;
	while(layer !=NULL && var == NULL){
		var = get_node(layer->root, name);
		layer = layer-> previous;
	}
	return var;
}


// insert a variable in the current context
int set_var_layer(struct _layer * layer, char* name, struct _variable * var){
	CHK(layer);
	CHK(name);
	CHK(var);
	int ret = set_node(layer->root, name, var);
	if(ret== EXIT_FAILURE)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}


// to exit a local context
struct _layer * close_layer( struct _layer * layer){
	CHK(layer);
	struct _layer * father = layer->previous;
	del_tree(layer->root);
	free(layer);
	layer= father;
	CHK(layer);
	exit(1);
	return layer;
}


//free all layers
int delete_layer( struct _layer * layer ){
	CHK(layer);
	struct _layer * l = close_layer(layer);
	if (l == NULL)
		return EXIT_SUCCESS;
	return delete_layer(l);
}


/*
EXAMPLE OF USE


struct _layer * my_layer=init_layer();
	set_var_layer( my_layer, name1 , var1);
	my_layer= add_layer(my_layer);
	assert(get_var_layer(my_layer , invalid)==NULL);
	assert(get_var_layer(my_layer , name1) !=NULL);

	my_layer= add_layer(my_layer);
	my_layer= close_layer(my_layer);
	delete_layer(my_layer);

*/
