#ifndef STRUCTS_H
#define STRUCTS_H

// enum _affectation{
// 	AFF,
// 	ADD,
// 	MUL,
// 	SUB
// };

// Union des types de valeurs possible
union _value {
	int ival;
	float fval;
};

// Type de variables possible
enum _type {
	INT_TYPE=0,
	FLOAT_TYPE,
	VOID_TYPE
};

// Structure manipulée par les actions sémantiques
struct _variable {
	enum _type *type;
	union _value *value;
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

// Bloc d'exécution courant
struct _layer {
	struct _node *root;
	struct _layer *previous;
};



#endif //STRUCTS_H