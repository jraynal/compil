#include "sem_actions.h"
#include "includes/tree.h"
#include "includes/list.h"
#include <stdlib.h>
#include <stdio.h>

#define LLVM( string ) fprintf(stdout,##string); fprintf(stdout,"\n");

#define CHK(truc) do{if(truc == NULL) perror(#truc); exit(1);}while(0)

int new_reg(){
	static int i =0;
	i++;
	return i;
}

struct _attribute newAttribute(char * id){
	struct _attribute a;
	a.regNum = new_reg();
	a.type = UNKNOWN;
	// a.code = "";
	a.identifier= id;
	return a;
}


struct _variable * varCreate(enum _type type,	union _value value){
	struct _variable* var = malloc(sizeof(struct _variable));
	if(var){
		var->type = type;
		var->value = value;
		// insertElmnt(var,garbageCollector);
	}
	else
		fprintf(stderr, "No created variable\n");
	return var;
}

struct _attribute getVar(char* nom,struct _node* htab){
	CHK(htab);
	char dest [100];
	sprintf(dest,"/%s",nom);
	struct _variable * var =NULL;
	var = get_node(htab,dest);
	CHK (var);
	struct _attribute a = newAttribute((char*)dest);
	a.type = var->type;
	a.code = "%%%d =load %s %s ";

	return a;
}

struct _attribute newInt(int i){
	struct _attribute a = newAttribute("/");
	a.type = INT_TYPE;
	a.code = "%%%d  = add i32 %d, 0;\n";
	return a;
}


struct _attribute newFloat(float f){
	struct _attribute a = newAttribute("/");
	a.type = FLOAT_TYPE;
	a.code = "%%%d  = fadd float %g, 0.0 ;\n";
	return a;
}

struct _attribute simpleFuncall(char * funName){
	struct _attribute a = newAttribute(funName);
	a.type = FLOAT_TYPE;
	a.code="call i32 ()* @%s ()\n";
	return a;

}


struct _attribute multipleFuncall(char * funName,struct _list * l){
			//TODO
	return newAttribute("");

}


struct _attribute varIncr(char * name){

	return newAttribute("");

}

struct _attribute varDecr(char * name){

	return newAttribute("");

}


struct _attribute getValArray(struct _attribute array, struct _attribute i){
	return newAttribute("");

}



struct _list * expressionList(struct _attribute a){
	return init_list();

}


struct _list * insertExpr(struct _attribute a ,struct _list * l){
	return init_list();
}



struct _attribute prefixedVarIncr(struct _attribute attr){
	return newAttribute("");

}


struct _attribute prefixedVarDecr(struct _attribute attr){
	return newAttribute("");

}



// struct _variable * varCreateInt(int i){

// 	struct _variable* var = malloc(sizeof(struct _variable));
// 	if(var){
// 		var->type = INT_TYPE;
// 		var->value.ival = i;
// 		insertElmnt(var,garbageCollector);
// 	}else
// 	fprintf(stderr, "No created variable\n");

// 	return var;
// }

// struct _variable * varCreateFloat(float i){

// 	struct _variable* var = malloc(sizeof(struct _variable));
// 	if(var){
// 		var->type = FLOAT_TYPE;
// 		var->value.fval = i;
// 		insertElmnt(var,garbageCollector);
// 	}else 
// 	fprintf(stderr, "No created variable\n");

// 	return var;
// }

struct _attribute binOp(struct _attribute a1,struct _attribute a2,char* intOp, char * floatOp){
	if (a1.type != a2.type){
		perror("invalid operation");
		exit(1);
	}
	struct _attribute a = newAttribute(a1.identifier);

	switch(a1.type){
		case INT_TYPE : 
		a.type = INT_TYPE;
		a.code="%%%d = %s i32 %%%d, i32 %%%d; \n";	
		break;
		case FLOAT_TYPE:
		a.type = FLOAT_TYPE;
		a.code="%%%d = %s float %%%d, float %%%d; \n";	
		break;
		default: 
		perror("invalid operation");
		exit(1);

	}
	return	a;
}


struct _attribute multiply(struct _attribute a1,struct _attribute a2){
	return binOp(a1,a2,"mul","fmul");
}

struct _attribute divide(struct _attribute a1,struct _attribute a2){
	return binOp(a1,a2,"sdiv","fdiv");
}

struct _attribute add(struct _attribute a1,struct _attribute a2){
	return binOp(a1,a2,"add","fadd");
}

struct _attribute sub(struct _attribute a1 ,struct _attribute a2){
	return binOp(a1,a2,"sub","fsub");
}


// struct _variable *incr(struct _variable *a){
// 	if (!a)
// 		return NULL;
// 	switch(a->type){
// 		case INT_TYPE :
// 		a->value.ival++;
// 		break;
// 		case FLOAT_TYPE : 
// 		a->value.fval++;
// 		break;
// 		default: 
// 		return NULL;
// 	}
// 	return a;
// }
// struct _variable *decr(struct _variable *a){
// 	if (!a)
// 		return NULL;
// 	switch(a->type){
// 		case INT_TYPE :
// 		a->value.ival--;
// 		break;
// 		case FLOAT_TYPE : 
// 		a->value.fval--;
// 		break;
// 		default: 
// 		return NULL;
// 	}
// 	return a;
// }


struct _attribute neg(struct _attribute a1){
	struct _attribute a = newAttribute(a1.identifier);
	switch(a.type){
		case INT_TYPE : 
		a.type = INT_TYPE;
		a.code = "%%%d = sub i32 0 , %%%d" ;
		break;
		case FLOAT_TYPE:
		a.type = FLOAT_TYPE;
		a.code = "%%%d = fsub float 0.0 , %%%d" ;
		break;
		default:
		perror("invalid operation");
		exit(1);
	}
	return a;
}


struct _attribute cmp(struct _attribute a1 ,struct _attribute a2 , char* intConditionCode,  char* floatConditionCode ){
	if (a1.type != a2.type){
		perror("invalid operation");
		exit(1);
	}
	struct _attribute a = newAttribute(a1.identifier);

	switch(a1.type){
		case INT_TYPE : 
		a.type = INT_TYPE;
		a.code="%%%d = icmp %s i32 %%%d, i32 %%%d; \n";	
		break;
		case FLOAT_TYPE:
		a.type = FLOAT_TYPE;
		a.code="%%%d = fcmp %s float %%%d, float %%%d; \n";	
		break;
		default: 
		perror("invalid operation");
		exit(1);
	}
	return	a;

}

struct _attribute l_op (struct _attribute a1 ,struct _attribute a2 ){
	return cmp (a1,a2,"slt","ult");
	// if (!a || !b)
	// 	return NULL;
	// float valA = a->value.fval,valB = b->value.fval;
	// if(a->type == INT_TYPE) 
	// 	valA = (float)a->value.ival;
	// if(b->type == INT_TYPE) 
	// 	valB = (float)b->value.ival;
	// union _value val;
	// val.ival= valA < valB;
	// struct _variable * var = varCreate(INT_TYPE,val);
	// // varFree(a);
	// // varFree(b);
	// return var;
}

struct _attribute g_op (struct _attribute a1 ,struct _attribute a2 ){
	return cmp (a1,a2,"sgt","ugt");

	// if (!a || !b)
	// 	return NULL;
	// float valA = a->value.fval,valB = b->value.fval;
	// if(a->type == INT_TYPE) 
	// 	valA = (float)a->value.ival;
	// if(b->type == INT_TYPE) 
	// 	valB = (float)b->value.ival;
	// union _value val;
	// val.ival= valA > valB;
	// struct _variable * var = varCreate(INT_TYPE,val);
	// // varFree(a);
	// // varFree(b);
	// return var;
}

struct _attribute le_op (struct _attribute a1 ,struct _attribute a2 ){
	return cmp (a1,a2,"sle","ule");

	// if (!a || !b)
	// 	return NULL;
	// float valA = a->value.fval,valB = b->value.fval;
	// if(a->type == INT_TYPE) 
	// 	valA = (float)a->value.ival;
	// if(b->type == INT_TYPE) 
	// 	valB = (float)b->value.ival;
	// union _value val;
	// val.ival= valA <= valB;
	// struct _variable * var = varCreate(INT_TYPE,val);
	// // varFree(a);
	// // varFree(b);
	// return var;
}

struct _attribute ge_op (struct _attribute a1 ,struct _attribute a2 ){
	return cmp (a1,a2,"sge","uge");

	// if (!a || !b)
	// 	return NULL;
	// float valA = a->value.fval,valB = b->value.fval;
	// if(a->type == INT_TYPE) 
	// 	valA = (float)a->value.ival;
	// if(b->type == INT_TYPE) 
	// 	valB = (float)b->value.ival;
	// union _value val;
	// val.ival= valA >= valB;
	// struct _variable * var = varCreate(INT_TYPE,val);
	// // varFree(a);
	// // varFree(b);
	// return var;
}

struct _attribute ne_op (struct _attribute a1 ,struct _attribute a2 ){
	return cmp (a1,a2,"ne","une");

	// if (!a || !b)
	// 	return NULL;
	// float valA = a->value.fval,valB = b->value.fval;
	// if(a->type == INT_TYPE) 
	// 	valA = (float)a->value.ival;
	// if(b->type == INT_TYPE) 
	// 	valB = (float)b->value.ival;
	// union _value val;
	// val.ival= valA != valB; // ?? c'est quoi cette ligne?
	// struct _variable * var = varCreate(INT_TYPE,val);
	// // varFree(a);
	// // varFree(b);
	// return var;
}

struct _attribute eq_op (struct _attribute a1 ,struct _attribute a2 ){
	return cmp (a1,a2,"eq","ueq");

	// if (!a || !b)
	// 	return NULL;
	// float valA = a->value.fval,valB = b->value.fval;
	// if(a->type == INT_TYPE) 
	// 	valA = (float)a->value.ival;
	// if(b->type == INT_TYPE) 
	// 	valB = (float)b->value.ival;
	// union _value val;
	// val.ival= valA == valB;
	// struct _variable * var = varCreate(INT_TYPE,val);
	// // varFree(a);
	// // varFree(b);
	// return var;
}

void affectValue (struct _attribute varName,enum _affectation how,struct _attribute value){
	// if(!toModify){
	// 	fprintf(stderr,"Invalid argument %s:%d(%s)\n",__FILE__,__LINE__,__func__);
	// 	return;
	// }
	// if(!withWhat){
	// 	fprintf(stderr,"Invalid argument %s:%d(%s)\n",__FILE__,__LINE__,__func__);
	// 	return;
	// }
	// if (toModify->type != withWhat->type && withWhat->type != UNKNOWN){
	// 	fprintf(stderr,"(%s:%d,%s)ERROR Invalid type : %d and %d are not the same type\n",__FILE__,__LINE__,__func__,toModify->type,withWhat->type);
	// 	return;
	// }
	// // LLVM(store )
	// if (toModify->type == INT_TYPE){
	// 	// int i = new_reg	();
	// 	fprintf(stdout, " store i32 %d,i32 %s\n",withWhat->value.ival,toModify->name);
	// 	switch(how){
	// 		case 1:
	// 		toModify->value.ival *= withWhat->value.ival;
	// 		break;
	// 		case 2:
	// 		toModify->value.ival += withWhat->value.ival;
	// 		break;
	// 		case 3:
	// 		toModify->value.ival -= withWhat->value.ival;
	// 		break;
	// 		default:
	// 		toModify->value.ival = withWhat->value.ival;
	// 		fprintf(stderr,"int value (%d) affected\n",toModify->value.ival );
	// 		break;
	// 	}
	// }else if (toModify->type == FLOAT_TYPE){
	// 	fprintf(stdout, " store float %f, float %s\n",withWhat->value.fval,toModify->name);
	// 	switch(how){
	// 		case 1:
	// 		toModify->value.fval *= withWhat->value.fval;
	// 		break;
	// 		case 2:
	// 		toModify->value.fval += withWhat->value.fval;
	// 		break;
	// 		case 3:
	// 		toModify->value.fval -= withWhat->value.fval;
	// 		break;
	// 		default:
	// 		toModify->value.fval = withWhat->value.fval;
	// 		break;
	// 	}
	// }
	// else
	// 	fprintf(stderr, "unmatched type : %d\n", (int)toModify->type);
}


struct _variable * declareVar(char* nom,struct _node* htab){
	if(!htab)
		fprintf(stderr, "No htab\n"); 
	char dest[strlen(nom)+2];
	sprintf(dest,"/%s",nom);

	union _value val;
	val.ival =0;
	struct _variable * var = varCreate(UNKNOWN,val);
	set_node(htab,dest,var);
	var = get_node(htab,dest);
	if(var==NULL)
		fprintf(stderr, "Variable non set : %s\n",dest);
	fprintf(stderr, "declaration de %s\n",nom);
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

void setTypeList(struct _list * list, enum _type t){
	if (is_empty(list)){
		fprintf(stderr, "(%s:%d)ERROR No variable for this type declaration\n",__FILE__,__LINE__);
		return;
	}else{
		while(! is_empty(list)){
			setType(list->head->value,t);
			removeElmnt(list->head->value,list);
		}
	}
	del_list(list);
}
