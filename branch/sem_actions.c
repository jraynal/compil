#include "sem_actions.h"

#define LLVM( string ) fprintf(stdout,##string); fprintf(stdout,"\n");

#define CHK(truc) do{if(truc == NULL) perror(#truc); exit(1);}while(0)

const char *itoa(int i) {
	char *c=NULL;
	int len=0;
	len=snprintf(c,len,"%d",i);
	c=(char*)malloc(len+1);
	snprintf(c,len+1,"%d",i);
	return c;
}

const char *new_reg(){
	static int i =0;
	i++;
	return itoa(i);
}

void addcode(struct _code c,char* str,...){
	//TODO (tad)
	return;
}
void initCode(struct _code * c ){
	//TODO (tad)
	c->text="";
	return;
}



char* strOfNametype(enum _type t){
	switch(t){
		case INT_TYPE :
		return "i32";
		break;
		case FLOAT_TYPE:
		return "float";
		break;
		default:
		return "";
	}
	perror("uncommon excecution");
	exit(1);
}

const char* officialName(const char* name){
	CHK(name);
	if (strcmp(name, "$accel")== 0)
		return "%accel";
	else return name;
}

struct _attribute newAttribute(const char * id){
	struct _attribute a;
	a.reg = new_reg();
	a.type = UNKNOWN;
	initCode(&a.code);
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

struct _attribute get_attr_from_tree(struct _node* htab,const char* name){
	CHK(htab);
	char dest [100];
	sprintf(dest,"/%s",name);
	struct _variable * var =NULL;
	var = get_node(htab,dest);
	CHK (var);
	struct _attribute a = newAttribute(name);
	a.type = var->type;
	return a;
}


struct _attribute getVar(const char* name,struct _node* htab){
	struct _attribute a = get_attr_from_tree(htab,name);
	addcode(a.code,"%%%s =load %s %s ",a.reg,strOfNametype(a.type),officialName(name));

	return a;
}

struct _attribute varIncr(const char * name,struct _node* htab){
	struct _attribute a = get_attr_from_tree(htab,name);
	const char * official_name = officialName(name);
	char * str_type = strOfNametype(a.type);
	addcode(a.code,"%%%s =load %s %s ",a.reg,str_type,official_name);
	
	const char *reg = new_reg();
	switch(a.type){
		case INT_TYPE : 
			addcode(a.code,"%%%s =add %s %%%s, i32 1",reg,str_type,a.reg);
			addcode(a.code,"store %s %%%s, %s %s ",str_type,reg,str_type,official_name);
			break;
		case FLOAT_TYPE :
			addcode(a.code,"%%%s = fadd %s %%%s, float 1.0",reg,str_type,a.reg);
			addcode(a.code,"store %s %%%s, %s %s ",str_type,reg,str_type,official_name);
			break; 
		default:
			break;

	}
	// addcode(a.code,"%%%s =add %s %%%s, %s",a.reg,strOfNametype(var->type),officialName(nom));
	// addcode(a.code,"%%%s =load %s %s ",a.reg,strOfNametype(var->type),officialName(nom));
	return a;

}

struct _attribute varDecr(const char * name,struct _node* htab){

	return newAttribute("");

}

struct _attribute simpleFuncall(struct _node* htab,const char * funName){
	struct _attribute a = get_attr_from_tree(htab,funName);
	addcode(a.code,"call  %s @%s ()\n",strOfNametype(a.type), a.identifier);
	return a;

}


struct _attribute multipleFuncall(struct _node* htab,const char * funName,struct _list * l){
	struct _attribute a = get_attr_from_tree(htab,funName);
	//TODO
	return a;

}

struct _attribute newInt(int i){
	struct _attribute a = newAttribute("/");
	a.type = INT_TYPE;
	addcode(a.code,"%%%s  = add i32 %s, 0;\n",a.reg,i);
	return a;
}

struct _attribute newFloat(float f){
	struct _attribute a = newAttribute("/");
	a.type = FLOAT_TYPE;
	addcode(a.code,"%%%s  = fadd float %g, 0.0 ;\n",a.reg,f);
	return a;
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
		addcode(a.code,"%%%s = %s i32 %%%s, i32 %%%s; \n",a.reg,intOp,a1.reg,a2.reg);	
		break;
		case FLOAT_TYPE:
		a.type = FLOAT_TYPE;
		addcode(a.code,"%%%s = %s float %%%s, float %%%s; \n",a.reg,floatOp,a1.reg,a2.reg);	
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
		addcode(a.code,"%%%s = sub i32 0 , %%%s",a.reg,a1.reg) ;
		break;
		case FLOAT_TYPE:
		a.type = FLOAT_TYPE;
		addcode(a.code , "%%%s = fsub float 0.0 , %%%s",a.reg,a1.reg) ;
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
		addcode(a.code,"%%%s = icmp %s i32 %%%s, i32 %%%s; \n",a.reg,intConditionCode, a1.reg, a2.reg);	
		break;
		case FLOAT_TYPE:
		a.type = FLOAT_TYPE;
		addcode (a.code,"%%%s = fcmp %s float %%%s, float %%%s; \n",a.reg,floatConditionCode,a1.reg,a2.reg);	
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
	// 	fprintf(stderr,"Invalid argument %s:%s(%s)\n",__FILE__,__LINE__,__func__);
	// 	return;
	// }
	// if(!withWhat){
	// 	fprintf(stderr,"Invalid argument %s:%s(%s)\n",__FILE__,__LINE__,__func__);
	// 	return;
	// }
	// if (toModify->type != withWhat->type && withWhat->type != UNKNOWN){
	// 	fprintf(stderr,"(%s:%s,%s)ERROR Invalid type : %s and %s are not the same type\n",__FILE__,__LINE__,__func__,toModify->type,withWhat->type);
	// 	return;
	// }
	// // LLVM(store )
	// if (toModify->type == INT_TYPE){
	// 	// int i = new_reg	();
	// 	fprintf(stdout, " store i32 %s,i32 %s\n",withWhat->value.ival,toModify->name);
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
	// 		fprintf(stderr,"int value (%s) affected\n",toModify->value.ival );
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
	// 	fprintf(stderr, "unmatched type : %s\n", (int)toModify->type);
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


