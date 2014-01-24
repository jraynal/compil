#include "sem_actions.h"

#define LLVM( string ) fprintf(stdout,##string); fprintf(stdout,"\n");

#define CHK(truc) do{if(truc == NULL) {fprintf(stderr,"error in "#truc" at %s in %s line %d\n",__FILE__,__FUNCTION__,__LINE__);exit(EXIT_FAILURE);}}while(0)

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

char* strOfNametype(enum _type *t){
	switch(*t){
		case INT_TYPE :
		return "i32";
		break;
		case FLOAT_TYPE:
		return "float";
		break;
		default:
		return "";
	}
	fprintf(stderr,"uncommon execution at %s in %s line %d\n",__FILE__,__FUNCTION__,__LINE__);
	exit(1);
}

const char* officialName(const char* name){
	CHK(name);
	if (strcmp(name, "$accel")== 0)
		return "%accel";
	else return name;
}

void deleteAttribute(struct _attribute* a) {
	free(a);
}

struct _attribute *newAttribute(const char * id){
	struct _attribute *a=malloc(sizeof(struct _attribute));
	a->reg = new_reg(); // registre de la donnée
	a->addr= NULL; // ctxtesse de la donnée (identifiants uniquement)
	enum _type t = UNKNOWN;
	a->type = &t;
	a->code = initCode();
	a->identifier= id;
	CHK(a);
	return a;
}

struct _variable * varCreate(enum _type *type, const char *addr){
	struct _variable* var = malloc(sizeof(struct _variable));
	if(var){
		var->type = type;
		var->addr = addr;
		// insertElmnt(var,garbageCollector);
	}
	else
		fprintf(stderr, "No created variable\n");
	CHK(var);
	return var;
}

struct _attribute *get_attr_from_tree(struct _layer* ctxt,const char* name){
	CHK(ctxt);
	CHK(name);
	char dest [100];
	sprintf(dest,"/%s",name);
	/* Obtention de l'indentifiant */
	struct _variable *var = get_var_layer(ctxt,dest);
	if (var == NULL)
		fprintf(stderr, "ERROR : unknown variable %s\n",name);
	CHK (var);

	/* Creation de la liste d'attributs */
	struct _attribute *a = newAttribute(name);

	/* Chargement de l'identifiant */
	char * str_type = strOfNametype(a->type);
	addCode(a->code,"%%%s =load %s* %s ",a->reg,str_type,var->addr);	// chargement en mémoire pour identifiant de variable
	a->addr = var->addr;												// sauvegarde de l'ctxtresse pour tableaux par exemple
	a->type = var->type;
	CHK(a);
	return a;														// ecriture
}


struct _attribute *getVar(const char* name,struct _layer *ctxt) {
	struct _attribute *a = get_attr_from_tree(ctxt,name);
	CHK(a);
	return a;
}

struct _attribute *varIncr(const char * name,struct _layer* ctxt){
	struct _attribute *a = get_attr_from_tree(ctxt,name);
	char * str_type = strOfNametype(a->type);
	
	/* Creation du registre de sortie du calcul */
	const char *reg = new_reg();
	switch(*a->type){
		/* Addition d'entiers */
		case INT_TYPE :
			addCode(a->code,"%%%s =add %s %%%s, i32 1",reg,str_type,a->reg);
			break;
		/* Addition de flottants */
		case FLOAT_TYPE :
			addCode(a->code,"%%%s = fadd %s %%%s, float 1.0",reg,str_type,a->reg);
			break; 
		default:
			break;

	}
	/* Sauvegarde dans l'identifiant */
	addCode(a->code,"store %s %%%s, %s %s ",str_type,reg,str_type,a->addr);
	a->reg=reg;
	CHK(a);	
	return a;
}

struct _attribute *varDecr(const char * name,struct _layer* ctxt) {
	struct _attribute *a = get_attr_from_tree(ctxt,name);
	char * str_type = strOfNametype(a->type);
	/* Creation du registre de sortie du calcul */
	const char *reg = new_reg();
	switch(*a->type){
		/* decrementation d'entiers */
		case INT_TYPE :
			addCode(a->code,"%%%s =sub %s %%%s, i32 1",reg,str_type,a->reg);
			break;
		/* decrementation de flottants */
		case FLOAT_TYPE :
			addCode(a->code,"%%%s = fsub %s %%%s, float 1.0",reg,str_type,a->reg);
			break; 
		default:
			break;
	}
	addCode(a->code,"store %s %%%s, %s %s ",str_type,reg,str_type,a->addr);
	return a;
}

struct _attribute *simpleFuncall(struct _layer* ctxt,const char * funName){
	struct _attribute *a = get_attr_from_tree(ctxt,funName);
	addCode(a->code,"call  %s @%s ()\n",strOfNametype(a->type), a->identifier);
	CHK(a);
	return a;

}


struct _attribute *multipleFuncall(struct _layer* ctxt,const char * funName,struct _list * l){
	struct _attribute *a = get_attr_from_tree(ctxt,funName);
	//TODO
	CHK(a);
	return a;

}

struct _attribute *newInt(int i){
	struct _attribute *a = newAttribute("/");
	*a->type = INT_TYPE;
	addCode(a->code,"%%%s  = add i32 %s, 0;\n",a->reg,i);
	CHK(a);
	return a;
}

struct _attribute *newFloat(float f){
	struct _attribute *a = newAttribute("/");
	*a->type = FLOAT_TYPE;
	addCode(a->code,"%%%s  = fadd float %g, 0.0 ;\n",a->reg,f);
	CHK(a);
	return a;
}



struct _attribute *getValArray(struct _attribute *array, struct _attribute *i){
	struct _attribute *a = newAttribute("/");
	// Ne pas oublier le code des autres...
	a->code=fusionCode(array->code,i->code);
	a->type = array->type;
	/* retourne l'élément situé à i.reg * array.type de l'ctxtesse de base, donc le ième */
	addCode(a->code,"%%%s = getelementptr %%%s* %%%s, %%%s %%%s\n",a->reg,strOfNametype(array->type),array->addr,strOfNametype(array->type),i->reg);
	CHK(a);
	return a;
}


/* TODO: gestion propre des listes d'attributs */
struct _list * expression_list(struct _attribute *a){
	struct _list * list = init_list();
	insertElmnt(a,list);
	CHK(list);
	return list;
}


struct _list * insert_expr_list(struct _attribute *a ,struct _list * list){
	insertElmnt(a,list);
	CHK(list);
	return list;
}


/* Je pense que la grammaire gère le cas où l'incrémentation doit se faire avant... */
struct _attribute *prefixedVarIncr(struct _attribute *a){
	char * str_type = strOfNametype(a->type);
	
	/* Creation du registre de sortie du calcul */
	const char *reg = new_reg();
	switch(*a->type){
		/* Addition d'entiers */
		case INT_TYPE :
			addCode(a->code,"%%%s =add %s %%%s, i32 1",reg,str_type,a->reg);
			break;
		/* Addition de flottants */
		case FLOAT_TYPE :
			addCode(a->code,"%%%s = fadd %s %%%s, float 1.0",reg,str_type,a->reg);
			break; 
		default:
			break;

	}
	/* Sauvegarde dans l'identifiant */
	addCode(a->code,"store %s %%%s, %s %s ",str_type,reg,str_type,a->addr);
	a->reg=reg;
	CHK(a);
	return a;
}


struct _attribute *prefixedVarDecr(struct _attribute *a){
	char * str_type = strOfNametype(a->type);
	/* Creation du registre de sortie du calcul */
	const char *reg = new_reg();
	switch(*a->type){
		/* decrementation d'entiers */
		case INT_TYPE :
			addCode(a->code,"%%%s =sub %s %%%s, i32 1",reg,str_type,a->reg);
			break;
		/* decrementation de flottants */
		case FLOAT_TYPE :
			addCode(a->code,"%%%s = fsub %s %%%s, float 1.0",reg,str_type,a->reg);
			break; 
		default:
			break;
	}
	addCode(a->code,"store %s %%%s, %s %s ",str_type,reg,str_type,a->addr);
	CHK(a);
	return a;
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

struct _attribute *binOp(struct _attribute *a1,struct _attribute *a2,char* intOp, char * floatOp){
	if (*a1->type != *a2->type){
		fprintf(stderr,"uncommon execution at %s in %s line %d\n",__FILE__,__FUNCTION__,__LINE__);
		exit(1);
	}
	// struct _attribute *a = newAttribute(a1->identifier);
	// Je qu'il faut un nouvel identifiant...
	struct _attribute *a = newAttribute("/");

	// ne pas oublier de récupérer le code qui remonte...
	a->code=fusionCode(a1->code,a2->code);

	switch(*a1->type){
		case INT_TYPE : 
		*a->type = INT_TYPE;
		addCode(a->code,"%%%s = %s i32 %%%s, i32 %%%s; \n",a->reg,intOp,a1->reg,a2->reg);	
		break;
		case FLOAT_TYPE:
		*a->type = FLOAT_TYPE;
		addCode(a->code,"%%%s = %s float %%%s, float %%%s; \n",a->reg,floatOp,a1->reg,a2->reg);	
		break;
		default: 
		fprintf(stderr,"invalid operation at %s in %s line %d\n",__FILE__,__FUNCTION__,__LINE__);
		exit(1);

	}
	CHK(a);
	return	a;
}

/* ça c'est super malin, de vrai de vrai !! */
struct _attribute *multiply(struct _attribute *a1,struct _attribute *a2){
	return binOp(a1,a2,"mul","fmul");
}

struct _attribute *divide(struct _attribute *a1,struct _attribute *a2){
	return binOp(a1,a2,"sdiv","fdiv");
}

struct _attribute *add(struct _attribute *a1,struct _attribute *a2){
	return binOp(a1,a2,"add","fadd");
}

struct _attribute *sub(struct _attribute *a1 ,struct _attribute *a2){
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


struct _attribute *neg(struct _attribute *a1){
	struct _attribute *a = newAttribute(a1->identifier);
	switch(*a->type){
		case INT_TYPE : 
		*a->type = INT_TYPE;
		addCode(a->code,"%%%s = sub i32 0 , %%%s",a->reg,a1->reg) ;
		break;
		case FLOAT_TYPE:
		*a->type = FLOAT_TYPE;
		addCode(a->code , "%%%s = fsub float 0.0 , %%%s",a->reg,a1->reg) ;
		break;
		default:
		fprintf(stderr,"invalid operation at %s in %s line %d\n",__FILE__,__FUNCTION__,__LINE__);
		exit(1);
	}
	CHK(a);
	return a;
}


struct _attribute *cmp(struct _attribute *a1 ,struct _attribute *a2 , char* intConditionCode,  char* floatConditionCode ){
	if (*a1->type != *a2->type){
		fprintf(stderr,"invalid operation at %s in %s line %d\n",__FILE__,__FUNCTION__,__LINE__);
		exit(1);
	}
	// struct _attribute *a = newAttribute(a1->identifier);
	// Idem binOp...
	struct _attribute *a = newAttribute("/");
	// Encore une fois, y a du code qui remonte, on le stocke:
	a->code=fusionCode(a1->code,a2->code);
	switch(*a1->type){
		case INT_TYPE : 
		*a->type = INT_TYPE;
		addCode(a->code,"%%%s = icmp %s i32 %%%s, i32 %%%s; \n",a->reg,intConditionCode, a1->reg, a2->reg);	
		break;
		case FLOAT_TYPE:
		*a->type = FLOAT_TYPE;
		addCode (a->code,"%%%s = fcmp %s float %%%s, float %%%s; \n",a->reg,floatConditionCode,a1->reg,a2->reg);	
		break;
		default:
		fprintf(stderr,"invalid operation at %s in %s line %d\n",__FILE__,__FUNCTION__,__LINE__);
		exit(1);
	}
	CHK(a);
	return	a;
}

struct _attribute *l_op (struct _attribute *a1 ,struct _attribute *a2 ){
	return cmp (a1,a2,"slt","ult");
}

struct _attribute *g_op (struct _attribute *a1 ,struct _attribute *a2 ){
	return cmp (a1,a2,"sgt","ugt");
}

struct _attribute *le_op (struct _attribute *a1 ,struct _attribute *a2 ){
	return cmp (a1,a2,"sle","ule");
}

struct _attribute *ge_op (struct _attribute *a1 ,struct _attribute *a2 ){
	return cmp (a1,a2,"sge","uge");
}

struct _attribute *ne_op (struct _attribute *a1 ,struct _attribute *a2 ){
	return cmp (a1,a2,"ne","une");

}

struct _attribute *eq_op (struct _attribute *a1 ,struct _attribute *a2 ){
	return cmp (a1,a2,"eq","ueq");
}


// Ne se fait que lorqu'on a à faire à un identifiant.
// VOID ie terminal? on peut pas faire une affectation dans une expression?
void affectValue (struct _attribute *varName,enum _affectation how,struct _attribute *value){

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

// ctxt = arbre de recherche
struct _attribute *declareVar(char* nom,struct _layer* ctxt){
	//vérification de l'existence de l'ARBRE DE RECHERCHE
	CHK(ctxt);
	CHK(nom);
	char dest[strlen(nom)+2];
	sprintf(dest,"/%s",nom);
	struct _attribute *a=newAttribute(nom);
	// Modification du type de la variable et de l'ctxtese du pointeur par effet de bord.
	// On ne pourra alour la mémoire qu'en connaisant le type!
	struct _variable * var = varCreate(a->type,a->addr);

	// Ajout de la clef dans l'arbre
	set_var_layer(ctxt,dest,var);

	/* DEBUG SPOTTED */
	// Vérifiction de la présence de la clef
	// var = get_layer(ctxt,dest);
	// if(var==NULL)
	// 	fprintf(stderr, "Variable non set : %s\n",dest);
	// fprintf(stderr, "declaration de %s\n",nom);

	// on remonte à la règle du dessus qui devra faire l'allocation!!
	CHK(a);
	return a;
}

struct _attribute *allocate_id(struct _attribute *a, enum _type t) {
	CHK(a);
	setType(a,t);
	addCode(a->code,"%%%s = alloca %s",a->addr,strOfNametype(&t));
	return a;	
}

void setType(struct _attribute *a, enum _type t){
	CHK(a);
	// Modification dans l'arbre par effet de bord
	a->type = &t;
	return;
}

//?????????????????????????????????????????????????????????????????????????????????????????????????
//?????????????????????????????????????????????????????????????????????????????????????????????????
//?????????????????????????????????????????????????????????????????????????????????????????????????
//?????????????????????????????????????????????????????????????????????????????????????????????????
//?????????????????????????????????????????????????????????????????????????????????????????????????
void setTypeList(struct _list * list, enum _type t){
	if (is_empty(list)){
		fprintf(stderr, "(%s:%d)ERROR No variable for this type declaration\n",__FILE__,__LINE__);
		return;
	}else{
		while(! is_empty(list)){
			/* TODO: set type a besoin d'un attribut et non pas d'une variable */
			setType(list->head->value,t);
			removeElmnt(list->head->value,list);
		}
	}
	del_list(list);
}

struct _attribute *make_function(enum _type t , struct _attribute * name, struct _attribute * content){
	//TODO
	struct _attribute* ret = newAttribute("/");
	CHK(ret);
	return ret;
}


struct _attribute*  new_statement_list(struct _attribute * statement){
	CHK(statement);
	struct _attribute * ret= newAttribute("/");
	//TODO
	CHK(ret);
	return ret;
}


struct _attribute * insert_statement_list(struct _attribute * statement, struct _attribute * list){
	CHK(list);
	CHK(statement);
	struct _attribute * ret= newAttribute("/");
	//TODO
	CHK(ret);
	return ret;
}



void print(struct _attribute *a) {
	CHK(a);
	printCode(STDOUT_FILENO,a->code);
	deleteCode(a->code);
}

struct _attribute *selection(struct _attribute *cond, struct _attribute *then, struct _attribute *other) {
	/* TODO: cond avec cas sans else si other est à NULL*/
	return NULL;
}

struct _attribute *loop(struct _attribute *init, struct _attribute *cond, struct _attribute *ite, struct _attribute *body) {
	struct _attribute *a=newAttribute("/");
	a->code=init->code;
	/* TODO: effacer certaines parties pour un while quand init et ite sont à
	 * NULL */
	/* TODO: label  */
	selection(cond,body,NULL); // On teste et on fait au besoin
	a->code=fusionCode(a->code,ite->code);
	/* TODO: renvoie au label */
	return NULL;
}

struct _attribute *concat(struct _attribute *a1,struct _attribute *a2) {
	struct _attribute *a=newAttribute("/");
	a->code=fusionCode(a1->code,a2->code);
	CHK(a);

	return a;
}
