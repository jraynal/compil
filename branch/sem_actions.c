#include "sem_actions.h"

#define LLVM( string ) fprintf(stdout,##string); fprintf(stdout,"\n");

#define CHK(truc) do{if(truc == NULL) {fprintf(stderr,"FATAL ERROR in "#truc" at %s in %s line %d\n",__FILE__,__FUNCTION__,__LINE__);}}while(0)
#define INVALID_OP  do{fprintf(stderr,"FATAL ERROR : uncommon execution at %s in %s line %d\n",__FILE__,__FUNCTION__,__LINE__);}while(0)

#define T_TYPE(name,type) fprintf(stderr,"[VERIF] %s is an %s\n",name,strOfNametype(type));

const char *itoa(int i) {
//	LOG();
	char *c=NULL;
	int len=0;
	len=snprintf(c,len,"%d",i);
	c=(char*)malloc(len+1);
	snprintf(c,len+1,"%d",i);
	return c;
}

const char *new_reg(){
//	LOG();
	static int i =0;
	return itoa(i++);//get_free_int(heap));
}

const char *new_label(){
//	LOG();
	static int i =0;
	return itoa(i++);
}
char* strOfNametype(enum _type t){
//	LOG();
	switch(t){
		case INT_ARRAY:
		case INT_FUNC:
		case INT_TYPE :
		return "i32";
		break;
		case FLOAT_FUNC:
		case FLOAT_ARRAY:
		case FLOAT_TYPE:
		return "float";
		break;
		default:
		return "";
	}
	INVALID_OP;
}

const char* officialName(const char* name){
//	LOG();
	CHK(name);
	if (strcmp(name, "_posx")== 0)
		return "$x";	
	if (strcmp(name, "_posy")== 0)
		return "$y";	
	if (strcmp(name, "_posz")== 0)
		return "$z";	
	if (strcmp(name, "_speed_x")== 0)
		return "$speedx";	
	if (strcmp(name, "_speed_y")== 0)
		return "$speedy";	
	if (strcmp(name, "_speed_z")== 0)
		return "$speedz";	
	if (strcmp(name, "_accel_x")== 0)
		return "$accelx";	
	if (strcmp(name, "_accel_y")== 0)
		return "$accely";	
	if (strcmp(name, "_accel_z")== 0)
		return "$accelz";	
	if (strcmp(name, "_enginerpm")== 0)
		return "$rpm";	
	if (strcmp(name, "_gear")== 0)
		return "$gear";	
	if (strcmp(name, "_steerCmd")== 0)
		return "$steer";
	if (strcmp(name, "$accel")== 0)
		return "%accel";

	else return name;
}

int is_ronly_var(const char* varname){
	if(strcmp(varname,"_posx")==0 || strcmp(varname,"_posy")==0 || strcmp(varname,"_posz")==0)
		return 1;
	if(strcmp(varname,"_speed_x")==0 || strcmp(varname,"_speed_z")==0 || strcmp(varname,"_speed_z")==0)
		return 1;
	if(strcmp(varname,"_accel_x")==0 || strcmp(varname,"_accel_y")==0 || strcmp(varname,"_accel_z")==0)
		return 1;
	if(strcmp(varname,"_enginerpm")==0 || strcmp(varname,"_gear")==0 || strcmp(varname,"/")==0)
		return 1;
	return 0;
}


int match_type(struct _attribute * a1 , struct _attribute * a2){
	CHK(a1);
	CHK(a2);
	int ret;
	ret = (a1->type == a2->type);
	ret = ret || (a1->type == UNKNOWN && (a2->type == UNKNOWN || a2->type == INT_TYPE ||  a2->type == FLOAT_TYPE ||  a2->type == VOID_TYPE));
	ret = ret || (a1->type == UNKNOWN_FUNC && (a2->type == UNKNOWN_FUNC || a2->type == INT_FUNC ||  a2->type == FLOAT_FUNC ||  a2->type == VOID_FUNC));
	ret = ret || (a1->type == UNKNOWN_ARRAY && (a2->type == UNKNOWN_ARRAY || a2->type == INT_ARRAY ||  a2->type == FLOAT_ARRAY ));
	ret = ret || (a2->type == UNKNOWN && (a1->type == UNKNOWN || a1->type == INT_TYPE ||  a1->type == FLOAT_TYPE ||  a1->type == VOID_TYPE));
	ret = ret || (a2->type == UNKNOWN_FUNC && (a1->type == UNKNOWN_FUNC || a1->type == INT_FUNC ||  a1->type == FLOAT_FUNC ||  a1->type == VOID_FUNC));
	ret = ret || (a2->type == UNKNOWN_ARRAY && (a1->type == UNKNOWN_ARRAY || a1->type == INT_ARRAY ||  a1->type == FLOAT_ARRAY ));
	
	return ret;
}

void deleteAttribute(struct _attribute* a) {
//	LOG();
	//free_int(heap,atoi(a->reg));
	if(a)
		free(a);
}

struct _attribute *newAttribute(const char * id){
//	LOG();
	struct _attribute *a=malloc(sizeof(struct _attribute));
	a->reg = new_reg(); // registre de la donnée
	a->addr= NULL; // ctxtesse de la donnée (identifiants uniquement)
	a->type= UNKNOWN;
	a->code = initCode();
	a->identifier= id;
	a->arguments = NULL;
	a->size=0;
	CHK(a);
	return a;
}

struct _variable * varCreate(enum _type type, const char *addr){
	LOG();
	T_TYPE(addr,type);
	struct _variable* var = malloc(sizeof(struct _variable));
	if(var){
		var->type = type;
		var->addr = addr;
	}
	else
		fprintf(stderr, "No created variable\n");
	CHK(var);
	return var;
}

struct _attribute *get_attr_from_context(struct _layer* ctxt,const char* name){
	LOG();
	CHK(ctxt);
	CHK(name);
	char dest [100];
	sprintf(dest,"/%s",name);
	/* Obtention de l'indentifiant */
	struct _variable *var = get_var_layer(ctxt,dest);
	CHK (var);

	/* Creation de la liste d'attributs */
	struct _attribute *a = newAttribute(name);

	/* Chargement de l'identifiant */
	a->addr = var->addr;												// sauvegarde de l'ctxtresse pour tableaux par exemple
	a->type = var->type;
	char * str_type = strOfNametype(a->type);
	addCode(a->code,"%%%s =load %s* %s \n",a->reg,str_type,var->addr);	// chargement en mémoire pour identifiant de variable
	CHK(a);
	T_TYPE(name,a->type);
	return a;														// ecriture
}

/* TODO: utiliser getVar pour faire un load en llvm et son père pour les manipulation simples*/
struct _attribute *getVar(const char* name,struct _layer *ctxt) {
	return get_attr_from_context(ctxt,name);
}

struct _attribute *varIncr(const char * name,struct _layer* ctxt){
	LOG();
	struct _attribute *a = get_attr_from_context(ctxt,name);
	char * str_type = strOfNametype(a->type);
	
	/* Creation du registre de sortie du calcul */
	const char *reg = new_reg();
	switch(a->type){
		/* Addition d'entiers */
		case INT_TYPE :
			addCode(a->code,"%%%s =add %s %%%s, i32 1\n",reg,str_type,a->reg);
			break;
		/* Addition de flottants */
		case FLOAT_TYPE :
			addCode(a->code,"%%%s = fadd %s %%%s, float %e\n",reg,str_type,a->reg,1.0);
			break; 
		default:
			break;

	}
	/* Sauvegarde dans l'identifiant */
	addCode(a->code,"store %s %%%s, %s %s\n",str_type,reg,str_type,a->addr);
	// a->reg=reg;
	a->identifier="/";
	CHK(a);	
	return a;
}

struct _attribute *varDecr(const char * name,struct _layer* ctxt) {
	LOG();
	struct _attribute *a = get_attr_from_context(ctxt,name);
	char * str_type = strOfNametype(a->type);
	/* Creation du registre de sortie du calcul */
	const char *reg = new_reg();
	switch(a->type){
		/* decrementation d'entiers */
		case INT_TYPE :
			addCode(a->code,"%%%s =sub %s %%%s, i32 1\n",reg,str_type,a->reg);
			break;
		/* decrementation de flottants */
		case FLOAT_TYPE :
			addCode(a->code,"%%%s = fsub %s %%%s, float %e\n",reg,str_type,a->reg,1.0);
			break; 
		default:
			break;
	}
	addCode(a->code,"store %s %%%s, %s %s\n",str_type,reg,str_type,a->addr);
	return a;
}

struct _attribute *simpleFuncall(struct _layer* ctxt,const char * funName){
	LOG();
	struct _attribute *a = get_attr_from_context(ctxt,funName);
	addCode(a->code,"call  %s @%s ()\n",strOfNametype(a->type), a->identifier);
	CHK(a);
	return a;

}


struct _attribute *multipleFuncall(struct _layer* ctxt,const char * funName,struct _list * list){
	LOG();
	CHK(ctxt);
	CHK(funName);
	CHK(list);
	struct _attribute *a = get_attr_from_context(ctxt,funName);
	CHK(a);
	struct _attribute *  argument;
	addCode(a->code,"call  %s @%s (",strOfNametype(a->type), a->identifier);
		while(!is_empty(list)){
			argument = (struct _attribute *) list->tail->value;
			addCode(a->code,", %s %%%d",strOfNametype(argument->type),argument->reg);// LLVM
			removeElmnt(argument,list);
		}
		addCode(a->code,"\n)");
		del_list(list);
	CHK(a);
	return a;
}

struct _attribute *newInt(int i){
	LOG();
	struct _attribute *a = newAttribute("/");
	a->type = INT_TYPE;
	
	addCode(a->code,"%%%s  = add i32 %d, 0\n",a->reg,i);
	CHK(a);
	return a;
}

struct _attribute *newFloat(float f){
	LOG();
	struct _attribute *a = newAttribute("/");
	a->type = FLOAT_TYPE;
	addCode(a->code,"%%%s  = fadd float %e, %e \n",a->reg,f,0.0);
	CHK(a);
	return a;
}


/* TODO: on ne peut pas affecter de valeurs dans un tableau à l'heure actuelle, juste les lire :S */
struct _attribute *getValArray(struct _attribute *array, struct _attribute *i){
	LOG();
	// Ne pas oublier le code des autres...
	if(i->type!=INT_TYPE)
		CHK(NULL);
	array->code=fusionCode(array->code,i->code);
	/* retourne l'élément situé à i.reg * array.type de l'ctxtesse de base, donc le ième */
	addCode(array->code,"%%%s = getelementptr %%%s* %%%s, %%%s %%%s\n",array->reg,strOfNametype(array->type),array->addr,strOfNametype(array->type),i->reg);
	array->type%=4;
	deleteAttribute(i);
	return array;
}

struct _list * expression_list(struct _attribute *a){
	LOG();
	struct _list * list = init_list();
	insertElmnt(a,list);
	CHK(list);
	return list;
}


struct _list * insert_expr_list(struct _attribute *a ,struct _list * list){
	LOG();
	insertElmnt(a,list);
	CHK(list);
	return list;
}

/* Je pense que la grammaire gère le cas où l'incrémentation doit se faire avant... */
struct _attribute *prefixedVarIncr(struct _attribute *a){
	LOG();
	char * str_type = strOfNametype(a->type);
	
	/* Creation du registre de sortie du calcul */
	const char *reg = new_reg();
	switch(a->type){
		/* Addition d'entiers */
		case INT_TYPE :
			addCode(a->code,"%%%s =add %s %%%s, i32 1\n",reg,str_type,a->reg);
			break;
		/* Addition de flottants */
		case FLOAT_TYPE :
			addCode(a->code,"%%%s = fadd %s %%%s, float 1.0\n",reg,str_type,a->reg);
			break; 
		default:
			break;

	}
	/* Sauvegarde dans l'identifiant */
	addCode(a->code,"store %s %%%s, %s %s\n",str_type,reg,str_type,a->addr);
	a->reg=reg;
	a->identifier="/";
	CHK(a);
	return a;
}


struct _attribute *prefixedVarDecr(struct _attribute *a){
	LOG();
	char * str_type = strOfNametype(a->type);
	/* Creation du registre de sortie du calcul */
	const char *reg = new_reg();
	switch(a->type){
		/* decrementation d'entiers */
		case INT_TYPE :
			addCode(a->code,"%%%s =sub %s %%%s, i32 1\n",reg,str_type,a->reg);
			break;
		/* decrementation de flottants */
		case FLOAT_TYPE :
			addCode(a->code,"%%%s = fsub %s %%%s, float 1.0\n",reg,str_type,a->reg);
			break; 
		default:
			break;
	}
	addCode(a->code,"store %s %%%s, %s %s \n",str_type,reg,str_type,a->addr);
	CHK(a);
	return a;
}

struct _attribute *binOp(struct _attribute *a1,struct _attribute *a2,char* intOp, char * floatOp){
	LOG();
	CHK(a1);
	CHK(a2);
	if (a1->type != a2->type){
		fprintf(stderr,"Different types: %s and %s\n",strOfNametype(a1->type),strOfNametype(a2->type));
		INVALID_OP;
	}
	// struct _attribute *a = newAttribute(a1->identifier);
	// Je qu'il faut un nouvel identifiant...
	struct _attribute *a = newAttribute("/");

	// ne pas oublier de récupérer le code qui remonte...
	a->code=fusionCode(a1->code,a2->code);

	switch(a1->type){
		case INT_TYPE : 
		a->type = INT_TYPE;
		addCode(a->code,"%%%s = %s i32 %%%s, i32 %%%s\n",a->reg,intOp,a1->reg,a2->reg);	
		break;
		case FLOAT_TYPE:
		a->type = FLOAT_TYPE;
		addCode(a->code,"%%%s = %s float %%%s, float %%%s\n",a->reg,floatOp,a1->reg,a2->reg);	
		break;
		default: 
		INVALID_OP;

	}
	deleteAttribute(a1);
	deleteAttribute(a2);
	CHK(a);
	return	a;
}

/* ça c'est super malin, de vrai de vrai !! */
struct _attribute *multiply(struct _attribute *a1,struct _attribute *a2){
	LOG();
	return binOp(a1,a2,"mul","fmul");
}

struct _attribute *divide(struct _attribute *a1,struct _attribute *a2){
	LOG();
	return binOp(a1,a2,"sdiv","fdiv");
}

struct _attribute *add(struct _attribute *a1,struct _attribute *a2){
	LOG();
	return binOp(a1,a2,"add","fadd");
}

struct _attribute *sub(struct _attribute *a1 ,struct _attribute *a2){
	LOG();
	return binOp(a1,a2,"sub","fsub");
}

struct _attribute *neg(struct _attribute *a){
	LOG();
	CHK(a);
	struct _attribute *na=newAttribute(a->identifier);
	na->type=a->type;
	switch(a->type){
		case INT_TYPE : 
		addCode(a->code,"%%%s = sub i32 0 , %%%s\n",na->reg,a->reg) ;
		break;
		case FLOAT_TYPE:
		addCode(a->code , "%%%s = fsub float %e , %%%s\n",na->reg,0.0,a->reg) ;
		break;
		default:
		INVALID_OP;
	}
	deleteAttribute(a);
	CHK(na);
	return na;
}


struct _attribute *cmp(struct _attribute *a1 ,struct _attribute *a2 , char* intConditionCode,  char* floatConditionCode ){
	LOG();
	CHK(a1);
	CHK(a2);
	if (a1->type != a2->type){
		INVALID_OP;
	}
	// struct _attribute *a = newAttribute(a1->identifier);
	// Idem binOp...
	struct _attribute *a = newAttribute("/");
	// Encore une fois, y a du code qui remonte, on le stocke:
	a->code=fusionCode(a1->code,a2->code);
	// les affectation de TYPE sont commentées car la comparaison renvoie un i1 et que ça doit rester transparent....
	switch(a1->type){
		case INT_TYPE : 
	//	a->type = INT_TYPE;
		addCode(a->code,"%%%s = icmp %s i32 %%%s, i32 %%%s\n",a->reg,intConditionCode, a1->reg, a2->reg);	
		break;
		case FLOAT_TYPE:
	//	a->type = FLOAT_TYPE;
		addCode (a->code,"%%%s = fcmp %s float %%%s, float %%%s\n",a->reg,floatConditionCode,a1->reg,a2->reg);	
		break;
		default:
		INVALID_OP;
	}
	CHK(a);
	deleteAttribute(a1);
	deleteAttribute(a2);
	return	a;
}

struct _attribute *l_op (struct _attribute *a1 ,struct _attribute *a2 ){
	LOG();
	return cmp (a1,a2,"slt","ult");
}

struct _attribute *g_op (struct _attribute *a1 ,struct _attribute *a2 ){
	LOG();
	return cmp (a1,a2,"sgt","ugt");
}

struct _attribute *le_op (struct _attribute *a1 ,struct _attribute *a2 ){
	LOG();
	return cmp (a1,a2,"sle","ule");
}

struct _attribute *ge_op (struct _attribute *a1 ,struct _attribute *a2 ){
	LOG();
	return cmp (a1,a2,"sge","uge");
}

struct _attribute *ne_op (struct _attribute *a1 ,struct _attribute *a2 ){
	LOG();
	return cmp (a1,a2,"ne","une");

}

struct _attribute *eq_op (struct _attribute *a1 ,struct _attribute *a2 ){
	LOG();
	return cmp (a1,a2,"eq","ueq");
}

struct _attribute *declareVar(char* nom){
	LOG();
	struct _attribute *a=newAttribute(nom);
	a->addr=a->identifier=nom;
	CHK(a);
	return a;
}

struct _attribute * declare_array(struct _attribute* array, int size){
	LOG();
	CHK(array);
	// On la rajoute
	array->type=UNKNOWN_ARRAY;
	array->size=size;
	return array;
}

struct _attribute *simple_declare_function(struct _attribute * func){
	LOG();
	CHK(func);
	CHK(my_ctxt);
	// my_ctxt=add_layer(my_ctxt);
	// On modifie le type mit par défaut par la déclaration de variables
	func->type = UNKNOWN_FUNC;
	// Liste d'arguments vide
	func->arguments = init_list();	
	// bout de code de déclaration:
	addCode(func->code,"@%s()",func->identifier);
	return func;
}


void my_add_layer(){
	my_ctxt=add_layer(my_ctxt);
	if(!is_empty(arg_to_add_in_contxt)){
		struct _attribute * head_attr = (struct _attribute *)arg_to_add_in_contxt->head->value;
		while(!is_empty(arg_to_add_in_contxt)){
			CHK(head_attr);
			//ctxt-formated name
			char dest[strlen(head_attr->identifier)+2];
			sprintf(dest,"/%s",head_attr->identifier);
			//insertion in new context
			struct _variable * var = varCreate(head_attr->type,head_attr->identifier);
			set_var_layer(my_ctxt,dest,var);
			removeElmnt(head_attr,arg_to_add_in_contxt);
		}
	}
}

struct _attribute *multiple_declare_function(struct _attribute * func , struct _list * args){
	LOG();
	CHK(func);
	CHK(args);
	CHK(my_ctxt);
	struct _attribute * attr;
	while(!is_empty(args)){
		attr = (struct _attribute *)args->head->value;
		insertElmnt(attr,arg_to_add_in_contxt);
		removeElmnt(attr,args);
	}
	del_list(args);
	func->type = UNKNOWN_FUNC;
	func->arguments = args;
	return func;
}


struct _attribute *allocate_id(struct _layer* ctxt, struct _attribute *a, enum _type t) {
	LOG();
	CHK(a);
	// Mise de la variable dans l'arbre
	CHK(ctxt);
	char dest[strlen(a->identifier)+2];
	sprintf(dest,"/%s",a->identifier);

	if(get_var_layer(ctxt,dest)!=NULL){
		fprintf(stderr, "FATAL ERROR %s already declared\n", a->identifier);
		return NULL;
	}

	// Maintenant on s'occupe du type et tout...
		//selon le type d'objet remonte : variable, fonction ou tableau
	switch(a->type){
		case UNKNOWN:
		a->type = t;
		addCode(a->code,"%%%s = alloca %s\n",a->addr,strOfNametype(t));
		break;
		case UNKNOWN_FUNC:
		switch(t){
			//selon le type a affecter (int float void...)
			case INT_TYPE :
			a->type = INT_FUNC;		
			break;
			case FLOAT_TYPE: 
			a->type = FLOAT_FUNC;
			break;
			case VOID_TYPE: 
			a->type = VOID_FUNC;
			break;
			default : 
			INVALID_OP;
		}
		break;
		//selon le type a affecter (int float ...)
		case UNKNOWN_ARRAY: 
		switch(t){
			case INT_TYPE :
			a->type = INT_ARRAY;		
			break;
			case FLOAT_TYPE: 
			a->type = FLOAT_ARRAY;
			break;
			default : 
			INVALID_OP;
		}
		addCode(a->code,
			"%%%s = alloca %s,%s %d\n",
			a->addr,strOfNametype(t),strOfNametype(t),a->size);
		break;
		default:
		INVALID_OP;
	}
	// Ajout de a variable avec le bon type dans l'arbre
	struct _variable * var = varCreate(a->type,a->identifier);
	set_var_layer(ctxt,dest,var);
	return a;	
}

struct _attribute * setTypeList(struct _list * list, enum _type t){
	LOG();
	CHK(list);
	struct _attribute* attr;
	struct _attribute* ret;
	ret = newAttribute("/");
	while(! is_empty(list)){
		attr = (struct _attribute *) (list->head->value);
		attr = allocate_id(my_ctxt,attr,t);
		ret= concat(ret,attr);
		removeElmnt(attr,list);
	}
	
	del_list(list);
	CHK(ret);
	return ret;
}


struct _attribute *make_function(enum _type t , struct _attribute * declaration, struct _attribute * content){
	LOG();
	CHK(declaration);CHK(content);
	struct _attribute* a= newAttribute("/");
	// Assemblage d'une définition de fonstion:
	// TODO check le type de declaration : doit etre une fonction
	if(t!=VOID_TYPE)
		addCode(a->code,"define %%%s ",strOfNametype(t));
	else
		addCode(a->code,"define ");
	a->code=fusionCode(a->code,declaration->code);
	addCode(a->code,"{\n");
	a->code=fusionCode(a->code,content->code);
	addCode(a->code,"}\n");
	CHK(a);
	return a;
}

void print(struct _attribute *a) {
	LOG();
	CHK(a);
	printCode(STDOUT_FILENO,a->code);
	deleteCode(a->code);
	deleteAttribute(a);
}

struct _attribute * emptyExpr(){
	struct _attribute * ret = newAttribute("/");
	CHK(ret);
	return ret;
}


struct _attribute *selection(struct _attribute *cond, struct _attribute *then, struct _attribute *other) {
	LOG();
	CHK(cond); CHK(then); 
	struct _attribute *a= newAttribute("/");
	const char *label1=new_label();
	const char *label2=new_label();
	const char *out_label=new_label();
	// On ajoute le code de la condition et on teste
	a->code=cond->code;
	a->code=addCode(cond->code,"br i1 %%%s, label %%%s label %%%s\n",cond->reg,label1,label2);
	// On ajoute le label et le then
	a->code=fusionCode(addCode(a->code,"label %%%s\n",label1),
				then->code);
	addCode(a->code,"br label %%%s\n",out_label);
	// On ajoute le label de other et si il y en a le code de
	addCode(a->code,"label %%%s\n",label2);
	if(other) {
		a->code=fusionCode(a->code,other->code);
		deleteAttribute(other);
	}
	addCode(a->code,"label %%%s\n",out_label);
	deleteAttribute(then);
	deleteAttribute(cond);
	CHK(a);
	CHK(a->code);
	return a;
}

struct _attribute *loop(struct _attribute *init, struct _attribute *cond, struct _attribute *ite, struct _attribute *body) {
	LOG();
	struct _attribute *a=newAttribute("/");
	const char *loop_label=new_label();
	if(init)
		a->code=init->code;
	a->code=fusionCode(addCode((init)?a->code:initCode(),"label %%%s\n",loop_label),cond->code);
	body->code=addCode(fusionCode(body->code,(ite)?ite->code:initCode()),"br label %%%s\n",loop_label);
	// Il faut conserver le registre du test pour la fonction selection
	a->reg=cond->reg;
	a=selection(a,body,NULL); // On teste et on fait au besoin
	if(init)
		deleteAttribute(init);
	deleteAttribute(cond);
	if(ite)
		deleteAttribute(ite);
	return a;
}

struct _attribute *concat(struct _attribute *a1,struct _attribute *a2) {
	LOG();
	CHK(a1);
	CHK(a2);
	CHK(a1->code);
	CHK(a2->code);
	struct _attribute *a=newAttribute("/");
	a->code=fusionCode(a1->code,a2->code);
	CHK(a);
	deleteAttribute(a1);
	deleteAttribute(a2);
	return a;
}

struct _attribute *assignment(struct _attribute *tgt, enum _affectation eg ,struct _attribute *ori){
	LOG();
	struct _attribute *a=NULL;
	struct _attribute *ret=newAttribute("/");
	CHK(tgt);
	if(is_ronly_var(tgt->identifier)){
		fprintf(stderr,"FATAL ERROR : This variable (%s) cannot be assigned",tgt->identifier);
		return NULL;
	}

	CHK(ori);
	if(match_type(tgt,ori)==0){
		fprintf(stderr, "FATAL ERROR : Unmatched types at %s in %s line %d\n",__FUNCTION__,__FILE__,__LINE__);
		return NULL;
	}
	switch(eg){
		case ADD:
			a=add(tgt,ori);
			break;
		case MUL:
			a=multiply(tgt,ori);
			break;
		case SUB:
			a=sub(tgt,ori);
			break;
		default:
			break;
	}
	char *type = strOfNametype(ori->type);
	// TRICKY: Là c'est la ligne ou on concatène tout le code reçut jusque là (et on croise les doigts que ça se fasse comme il faut :p)
	ret->code=addCode((a)?a->code:concat(tgt,ori)->code,
				"store %s %%%s, %s* %s\n",
						type,
						(a)?a->reg:ori->reg,
						type,
						tgt->addr);
	//deleteAttribute(tgt);
	deleteAttribute(a);
	//deleteAttribute(ori);
	return ret;
}

struct _attribute *return_jump(struct _attribute *a) {
	if(a) {
		addCode(a->code,"ret %s %%%s\n",a->type,a->reg);
	}
	else {
		a=newAttribute("/");
		addCode(a->code,"ret void\n");
	}
	CHK(a);
	return a;
}
