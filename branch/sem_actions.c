#include "sem_actions.h"
#include "includes/tree.h"
#include <stdlib.h>
#include <stdio.h>

#define LLVM( string ) fprintf(stdout,##string); fprintf(stdout,"\n");

int new_reg(){
	static int i =0;
	i++;
	return i;
}

struct _variable * varCreate(enum _type type,	union _value value){
	struct _variable* var = malloc(sizeof(struct _variable));
	if(var){
		var->type = type;
		var->value = value;
	}
	else
		fprintf(stderr, "No created variable\n");
	return var;
}

struct _variable * varCreateInt(int i){

	struct _variable* var = malloc(sizeof(struct _variable));
	if(var){
		var->type = INT_TYPE;
		var->value.ival = i;
	}else
	fprintf(stderr, "No created variable\n");

	return var;
}

struct _variable * varCreateFloat(float i){

struct _variable* var = malloc(sizeof(struct _variable));
if(var){
	var->type = FLOAT_TYPE;
	var->value.fval = i;
}else 
fprintf(stderr, "No created variable\n");

return var;
}

int varFree(struct _variable * a){
	if(a)
		free(a);
	else{
		fprintf(stderr, "Variable already freed\n");
		return EXIT_FAILURE;
	}
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
	if (a->type == UNKNOWN)     // b ne peut jamais être UNKNOW?
		a->type = b->type;		//faut donc caster la valeur de a avant de calculer
	else if (b->type != a->type && b->type != UNKNOWN)
		return NULL;
	if (a->type == INT_TYPE)
		a->value.ival -= b->value.ival; //  LLVM("%%d = sub i32 %%d, %%d",registre de sortie, a->value.ival, b->value.ival)
	else if(a->type == FLOAT_TYPE)		//  
		a->value.fval -= b->value.fval;	// LLVM("%%d = fsub float %%d, %%d",registre de sortie, a->value.fval, b->value.fval)
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
	val.ival= valA != valB; // ?? c'est quoi cette ligne?
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
	if(!toModify){
		fprintf(stderr,"Invalid argument %s:%d(%s)\n",__FILE__,__LINE__,__func__);
		return;
	}
	if(!withWhat){
		fprintf(stderr,"Invalid argument %s:%d(%s)\n",__FILE__,__LINE__,__func__);
		return;
	}
	if (toModify->type != withWhat->type && withWhat->type != UNKNOWN){
		fprintf(stderr,"(%s:%d,%s)ERROR Invalid type : %d and %d are not the same type\n",__FILE__,__LINE__,__func__,toModify->type,withWhat->type);
		return;
	}
	// LLVM(store )
	if (toModify->type == INT_TYPE){
		// int i = new_reg	();
		fprintf(stdout, " store i32 %d,i32 %s\n",withWhat->value.ival,toModify->name);
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
			fprintf(stderr,"int value (%d) affected\n",toModify->value.ival );
			break;
		}
	}else if (toModify->type == FLOAT_TYPE){
		fprintf(stdout, " store float %f, float %s\n",withWhat->value.fval,toModify->name);
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
	else
		fprintf(stderr, "unmatched type : %d\n", (int)toModify->type);
}

void print_debug(char *data) {
	int i;
	fprintf(stderr,">>>>>>>>>>>>>>>>>>>>");
	for(i=0;data[i]!='\0';i++)
		fprintf(stderr,"%c",data[i]);
	fprintf(stderr,"\n");
}

struct _variable * declareVar(char* nom,struct _node* htab){
	if(!htab)
		fprintf(stderr, "No htab\n"); 
	char dest[100];
	sprintf(dest,"/%s",nom);

	union _value val;
	val.ival =0;
	struct _variable * var = varCreate(UNKNOWN,val);
	set_node(htab,dest,var);
	var = get_node(htab,dest);
	if(var==NULL)
		fprintf(stderr, "Variable non set : %s\n",dest);
	if(strcmp(nom,"$accel")==0){
		var->name = "%accelCmd";
		// fprintf(stderr, "%s found \n",nom );
	}
	else {
		fprintf(stderr, "Unchanged name : %s(size:%d)\n",nom,(int)strlen(nom) );
		var->name = nom;
	}
	fprintf(stderr, "declaration de %s as %s\n",nom,var->name);
	return var;
}

struct _variable * getVar(char* nom,struct _node* htab){
	fprintf(stderr, "research of %s\n",nom );
	char dest [100];
	sprintf(dest,"/%s",nom);
	struct _variable * var =NULL;
	var = get_node(htab,dest);
	if (!var)
		fprintf(stderr, "%s (%d) Unreachable variable : %s\n",__FILE__,__LINE__,dest );
	return var;
}

void setType(struct _variable *var, enum _type t){
	if(! var){
		fprintf(stderr, "No value given : %s(%d)\n",__FILE__,__LINE__);
		return;
	}
	var->type = t;
	return;
}

// void setType(struct _list * list , enum _type type){

// }

struct _list * createList(){
	struct _list* list = malloc(sizeof(struct _list));
	return list;
}


