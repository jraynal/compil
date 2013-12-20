#include "sem_actions.h"
#include "structs.h"


struct _variable * varCreate(enum _type type,	union _value *value){

	return 0;
}

int varFree(struct _variable * a){


	return 0;
}

struct _variable *mul(struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;

	enum _type t = UNKNOWN;
	if (a->type == UNKNOWN)
		t = b->type;
	else if (b->type != a->type && b->type != UNKNOWN)
		return NULL;

	if (t == INT_TYPE){
		a->value->ival *= b->value->ival;
		a->value->fval = (float)a->value->ival;
	}
	else if(t == FLOAT_TYPE)
		a->value->fval *= b->value->fval;
	else
		return NULL;
	varFree(b);
	return a;
}

struct _variable *divide(struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;

	enum _type t = UNKNOWN;
	if (a->type == UNKNOWN)
		t = b->type;
	else if (b->type != a->type && b->type != UNKNOWN)
		return NULL;

	if (t == INT_TYPE){
		a->value->ival /= b->value->ival;
		a->value->fval = (float)a->value->ival;
	}
	else if(t == FLOAT_TYPE)
		a->value->fval /= b->value->fval;
	else
		return NULL;
	varFree(b);
	return a;
}

struct _variable *add(struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;

	enum _type t = UNKNOWN;
	if (a->type == UNKNOWN)
		t = b->type;
	else if (b->type != a->type && b->type != UNKNOWN)
		return NULL;

	if (t == INT_TYPE){
		a->value->ival += b->value->ival;
		a->value->fval = (float)a->value->ival;
	}
	else if(t == FLOAT_TYPE)
		a->value->fval += b->value->fval;
	else
		return NULL;
	varFree(b);
	return a;
}

struct _variable *sub(struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;

	enum _type t = UNKNOWN;
	if (a->type == UNKNOWN)
		t = b->type;
	else if (b->type != a->type && b->type != UNKNOWN)
		return NULL;

	if (t == INT_TYPE){
		a->value->ival -= b->value->ival;
		a->value->fval = (float)a->value->ival;
	}
	else if(t == FLOAT_TYPE)
		a->value->fval -= b->value->fval;
	else
		return NULL;
	varFree(b);
	
	return a;
}



struct _variable *incr(struct _variable *a){
	if (!a)
		return NULL;
	switch(a->type){
		case UNKNOWN:
	//pas de break puisqu'on va mettre a jour fval aussi...		
		case INT_TYPE :
		a->value->ival++;
	//pas de break puisqu'on va mettre a jour fval aussi...		
		case FLOAT_TYPE : 
		a->value->fval++;
		break;
		default: 
		return NULL;
	}
	return a;
}

struct _variable *decr(struct _variable *a){
	if (!a)
		return NULL;
	switch(a->type){
		case UNKNOWN:
	//pas de break puisqu'on va mettre a jour fval aussi...		
		case INT_TYPE :
		a->value->ival--;
	//pas de break puisqu'on va mettre a jour fval aussi...		
		case FLOAT_TYPE : 
		a->value->fval--;
		break;
		default: 
		return NULL;
	}
	return a;
}

struct _variable *neg(struct _variable *a){
	if (!a)
		return NULL;
	switch(a->type){
		case UNKNOWN:
	//pas de break puisqu'on va mettre a jour fval aussi...		
		case INT_TYPE :
		a->value->ival *= -1;
	//pas de break puisqu'on va mettre a jour fval aussi...		
		case FLOAT_TYPE : 
		a->value->fval *= -1;
		break;
		default: 
		return NULL;
	}
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

