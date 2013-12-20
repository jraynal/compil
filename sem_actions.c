#include "sem_actions.h"
#include "structs.h"


struct _variable * varCreate(enum _type type,	union _value *value){

return 0;
}

int varFree(struct _variable * a){


	return 0;
}

struct _variable *mul(struct _variable * a,struct _variable * b){
	if(a && b){
		enum _type t =-1;
		if (a->type)
			t = *(a->type);
		if (b->type)
			if (*(b->type)!=t && t!=-1)
				printf("Conflicting types\n");
				varFree(a);
				varFree(b);
				exit(-1);
			t = *(b->type);
		// switch (*(a->type)) 
		(a->value)-> ival *= (b->value)->ival;
		varFree(b);


	}
	return a;
}

struct _variable *divide(struct _variable * a,struct _variable * b){
	return a;
}

struct _variable *add(struct _variable * a,struct _variable * b){
	return a;
}

struct _variable *sub(struct _variable * a,struct _variable * b){
	return a;
}



struct _variable *incr(struct _variable *a){
	return a;
}

struct _variable *decr(struct _variable *a){
	return a;
}

struct _variable *neg(struct _variable *a){
	return a;
}


struct _variable * l_op (struct _variable * a,struct _variable * b){
	return a;
}

struct _variable * g_op (struct _variable * a,struct _variable * b){
	return a;
}

struct _variable * le_op (struct _variable * a,struct _variable * b){
	return a;
}

struct _variable * ge_op (struct _variable * a,struct _variable * b){
	return a;
}

struct _variable * ne_op (struct _variable * a,struct _variable * b){
	return a;
}

struct _variable * eq_op (struct _variable * a,struct _variable * b){
	return a;
}

