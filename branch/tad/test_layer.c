#include <assert.h>
#include "layer.h"

#ifndef DEBUG
#define DB(say)
#else
#define DB(say) fprintf(stderr,#say"\n");
#endif

#define TEST1 "my name is bob\n"
#define TEST2 "my name is michel and i am %d\n",12 

struct _variable *init_data(const char * val){
	struct _variable *var = (struct _variable*)malloc(sizeof(struct _variable));
	if(var ==NULL)
		printf("you failed!!\n");
	var->type=INT_TYPE;
	var->addr=val;
	return var;
}


int main() {
	DB(##################################################);
	DB(Test de Layer:);

	char * name1  = "/name1";
	char * invalid = "/invalid_path";
	struct _variable * var1 = init_data(name1);

	DB(Initialisation...)
	struct _layer * my_layer=init_layer();



	DB(Insertion...)
	set_var_layer( my_layer, name1 , var1);

	DB(New layer)
	my_layer= add_layer(my_layer);

	DB(Get...)
	assert(get_var_layer(my_layer , invalid)==NULL);
	DB(Get2...)
	assert(get_var_layer(my_layer , name1) !=NULL);

	DB(New layer)
	my_layer= add_layer(my_layer);

	DB(Close layer)
	my_layer= close_layer(my_layer);


	DB(destruction);
	delete_layer(my_layer);
	DB(##################################################);
	return EXIT_SUCCESS;
}