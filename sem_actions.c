#include "sem_actions.h"
#include <stdlib.h>
#include <stdio.h>
struct _variable * varCreate(enum _type type,	union _value value){
	struct _variable* var = malloc(sizeof(struct _variable));
	if(var){
		var->type = type;
		var->value = value;
		var->isFree =1;
	}
	return var;
}
int varFree(struct _variable * a){
	if(!a)
		free(a);
	else
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
struct _variable *mul(struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;
	if (a->type == UNKNOWN)
		a->type = b->type;
	else if (b->type != a->type && b->type != UNKNOWN)
		return NULL;
	if (a->type == INT_TYPE)
		a->value.ival *= b->value.ival;
	else if(a->type == FLOAT_TYPE)
		a->value.fval *= b->value.fval;
	else
		return NULL;
	varFree(b);
	return a;
}
struct _variable *divide(struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;
	if (a->type == UNKNOWN)
		a->type = b->type;
	else if (b->type != a->type && b->type != UNKNOWN)
		return NULL;
	if (a->type == INT_TYPE)
		a->value.ival /= b->value.ival;
	else if(a->type == FLOAT_TYPE)
		a->value.fval /= b->value.fval;
	else
		return NULL;
	varFree(b);
	return a;
}
struct _variable *add(struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;
	if (a->type == UNKNOWN)
		a->type = b->type;
	else if (b->type != a->type && b->type != UNKNOWN)
		return NULL;
	if (a->type == INT_TYPE)
		a->value.ival += b->value.ival;
	else if(a->type == FLOAT_TYPE)
		a->value.fval += b->value.fval;
	else
		return NULL;
	varFree(b);
	return a;
}
struct _variable *sub(struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;
	if (a->type == UNKNOWN)
		a->type = b->type;
	else if (b->type != a->type && b->type != UNKNOWN)
		return NULL;
	if (a->type == INT_TYPE)
		a->value.ival -= b->value.ival;
	else if(a->type == FLOAT_TYPE)
		a->value.fval -= b->value.fval;
	else
		return NULL;
	varFree(b);
	
	return a;
}
struct _variable *incr(struct _variable *a){
	if (!a)
		return NULL;
	switch(a->type){
		case INT_TYPE :
		a->value.ival++;
		break;
		case FLOAT_TYPE : 
		a->value.fval++;
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
		case INT_TYPE :
		a->value.ival--;
		break;
		case FLOAT_TYPE : 
		a->value.fval--;
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
		case INT_TYPE :
		a->value.ival *= -1;
		break;
		case FLOAT_TYPE : 
		a->value.fval *= -1;
		break;
		default: 
		return NULL;
	}
	return a;
}
struct _variable * l_op (struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;
	float valA = a->value.fval,valB = b->value.fval;
	if(a->type == INT_TYPE) 
		valA = (float)a->value.ival;
	if(b->type == INT_TYPE) 
		valB = (float)b->value.ival;
	union _value val;
	val.ival= valA < valB;
	struct _variable * var = varCreate(INT_TYPE,val);
	varFree(a);
	varFree(b);
	return var;
}
struct _variable * g_op (struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;
	float valA = a->value.fval,valB = b->value.fval;
	if(a->type == INT_TYPE) 
		valA = (float)a->value.ival;
	if(b->type == INT_TYPE) 
		valB = (float)b->value.ival;
	union _value val;
	val.ival= valA > valB;
	struct _variable * var = varCreate(INT_TYPE,val);
	varFree(a);
	varFree(b);
	return var;
}
struct _variable * le_op (struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;
	float valA = a->value.fval,valB = b->value.fval;
	if(a->type == INT_TYPE) 
		valA = (float)a->value.ival;
	if(b->type == INT_TYPE) 
		valB = (float)b->value.ival;
	union _value val;
	val.ival= valA <= valB;
	struct _variable * var = varCreate(INT_TYPE,val);
	varFree(a);
	varFree(b);
	return var;
}
struct _variable * ge_op (struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;
	float valA = a->value.fval,valB = b->value.fval;
	if(a->type == INT_TYPE) 
		valA = (float)a->value.ival;
	if(b->type == INT_TYPE) 
		valB = (float)b->value.ival;
	union _value val;
	val.ival= valA >= valB;
	struct _variable * var = varCreate(INT_TYPE,val);
	varFree(a);
	varFree(b);
	return var;
}
struct _variable * ne_op (struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;
	float valA = a->value.fval,valB = b->value.fval;
	if(a->type == INT_TYPE) 
		valA = (float)a->value.ival;
	if(b->type == INT_TYPE) 
		valB = (float)b->value.ival;
	union _value val;
	val.ival= valA != valB;
	struct _variable * var = varCreate(INT_TYPE,val);
	varFree(a);
	varFree(b);
	return var;
}
struct _variable * eq_op (struct _variable * a,struct _variable * b){
	if (!a || !b)
		return NULL;
	float valA = a->value.fval,valB = b->value.fval;
	if(a->type == INT_TYPE) 
		valA = (float)a->value.ival;
	if(b->type == INT_TYPE) 
		valB = (float)b->value.ival;
	union _value val;
	val.ival= valA == valB;
	struct _variable * var = varCreate(INT_TYPE,val);
	varFree(a);
	varFree(b);
	return var;
}


void affectValue (struct _variable * toModify,int how,struct _variable * withWhat ){
	if(!toModify || !withWhat){
		fprintf(stderr,"Invalid argument %s:%d(%s)\n",__FILE__,__LINE__,__func__);
		return;
	}
	if (toModify->type != withWhat->type && withWhat->type != UNKNOWN){
		fprintf(stderr,"Invalid type %s:%d(%s)\n",__FILE__,__LINE__,__func__);
		return;
	}
	if (toModify->type == INT_TYPE){
		switch(how){
			case 1:
			toModify->value.ival *= withWhat->value.ival;
			break;
			case 2:
			toModify->value.ival += withWhat->value.ival;
			break;
			case 3:
			toModify->value.ival -= withWhat->value.ival;
			break;
			default:
			toModify->value.ival = withWhat->value.ival;
			break;
		}
	}else if (toModify->type == FLOAT_TYPE){
		switch(how){
			case 1:
			toModify->value.fval *= withWhat->value.fval;
			break;
			case 2:
			toModify->value.fval += withWhat->value.fval;
			break;
			case 3:
			toModify->value.fval -= withWhat->value.fval;
			break;
			default:
			toModify->value.fval = withWhat->value.fval;
			break;
		}
	}
}

void setType(struct _list * list , enum _type type){

}

struct _list * createList(){
	struct _list* list = malloc(sizeof(struct _list));
	return list;
}