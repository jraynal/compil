#include "layer.h"


#define CHK(truc) do{if(truc == NULL) perror(#truc); exit(1);}while(0)

struct _layer * init_layer(){
	struct _layer * layer = malloc(sizeof(struct _layer));
	if (layer == NULL)
		exit(EXIT_FAILURE);
	layer-> root = init_tree();
	layer-> previous = NULL;
	return layer;
}

struct _layer * add_layer(struct _layer * layer){
	CHK(layer);
	struct layer * son = init_layer();
	CHK(son);
	son-> previous = layer;
	return son;
}

struct _variable * get_var_layer(struct _layer * layer , char * name){
	CHK(layer);
	CHK(name);
	struct _variable * var = get_node(layer->root, name);
	while(layer !=NULL && var == NULL){
		layer = layer-> previous;
		var = get_node(layer->root, name);
	}
	return var;
}

int set_var_layer(struct _layer * layer, char* name, struct _variable * var){
	CHK(layer);
	CHK(name);
	CHK(var);
	int ret = set_node(layer->root, name, var);
	if(ret)
	
}

int delete_layer( struct * _layer l ){


}


/*

struct _layer l = init_layer();
add_context(l);


insert_var(l,i);
insert_var(l,a);
gat_var(l,a);

	add_context(l);

	insert_var(i,b);
	get_var(l,i);

	close_context(l);


close_context(l);

delete_layer(l);




*/
