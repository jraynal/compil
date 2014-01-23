#ifndef STRUCTS_H
#define STRUCTS_H


// Type de variables possible
enum _type {
	INT_TYPE=0,
	FLOAT_TYPE,
	VOID_TYPE,
	UNKNOWN
};

/******Enum and struct ****/
enum _affectation {
	AFF=0,
	ADD,
	MUL,
	SUB
};

struct _string {
	char *text;
	int length;
	struct _string *next;
};

struct _code {
	struct _string *begin;
	struct _string *end;
};

struct _attribute {
	const char *reg;    // Valeurs
	const char *addr;   // Adresses
	enum _type *type;
	struct _code *code;
	const char * identifier;
};


/******************* Storage unit in the tree *********************/

// Union des types de valeurs possible
/* OBSOLETE */
union _value {
	int ival;
	float fval;
};


// Structure manipulée par les actions sémantiques
/* TODO: différencier variables R_ONLY ou RW */
/* TODO: champs de type d'objet: variable, tableau ou foction */
struct _variable {
	enum _type *type;
	const char *addr;
	//union _value value;
};


/***************************   Arbre de recherche   ***************************/

// Noeud de l'arbre de recherche, la racine n'aura jamais de variable
struct _node {
	char c;
	struct _variable *variable;
	struct _node *first_son;
	struct _node *last_son;
	struct _node *brother;
};

/******************* Context **************************/
// Bloc d'exécution courant
struct _layer {
	struct _node *root;
	struct _layer *previous;
};

/************ HEAP to minize the number of register **********/
struct _self_balancing_tree{
	int value;
	// number of node is the entire tree
	int weight;
	//left son
	struct _self_balancing_tree * left;
	//right son
	struct _self_balancing_tree * right;
};

struct _int_heap{
	// for each number i greater (or equal) than first_free , register i is free
	int first_free;
	// for each i in tree, register i is free
	struct _self_balancing_tree * tree;
};


/************List************/
struct _list
{
	int size;
	struct _nodeList* head ;
	struct _nodeList* tail;
}; 

struct _nodeList
{
	void* value;
	struct _nodeList * prev;
	struct _nodeList * next ;
};

/*******Global Variables ****/
// FAUX, il est spécifique à un contexte.
// struct _node* adr; 
extern struct _layer *my_ctxt;
struct _list * garbageCollector;

#endif //STRUCTS_H
