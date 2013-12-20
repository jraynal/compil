#ifndef STRUCTS_H
#define STRUCTS_H



enum _type{
	INT,
	FLOAT,
	VOID
};

union _value{
	int ival;
	float fval;
};

struct _variable{
	enum _type type;
	union _value value;
};

struct _node{
	char c;
	struct _variable *variable;
	struct _node *brother;
	struct _node *last_son;
	struct _node *sons;
};

struct _layer{
	struct _node *root;
	struct _layer *previous;
};

#endif
