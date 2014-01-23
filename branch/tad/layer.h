#ifndef LAYER_H
#define LAYER_H

#include <stdlib.h>
#include <stdio.h>
#include "structs.h"
#include "tree.h"

//create a new context
struct _layer * init_layer();


// add a new current context, the previous one will be the father
struct _layer * add_layer(struct _layer * layer);


// get the variable from the current context (or above if it does not exist)
struct _variable * get_var_layer(struct _layer * layer , char * name);


// insert a variable in the current context
int set_var_layer(struct _layer * layer, char* name, struct _variable * var);


// to exit a local context
struct _layer * close_layer( struct _layer * layer);


//free all layers
int delete_layer( struct _layer * layer );


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


# endif