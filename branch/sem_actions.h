#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/structs.h"

 //for dynamic allocation

struct _variable * varCreate(enum _type type,	union _value value);
// struct _attribute varCreateInt(int );
// struct _attribute varCreateFloat(float );


struct _attribute newInt(int i);
struct _attribute newFloat(float f);

struct _attribute simpleFuncall(char * funName);
struct _attribute multipleFuncall(char * funName,struct _list * l);
struct _attribute varIncr(char * name);
struct _attribute varDecr(char * name);

struct _attribute getValArray(struct _attribute array, struct _attribute i);

struct _list * expressionList(struct _attribute);
struct _list * insertExpr(struct _attribute a ,struct _list * l);

struct _attribute prefixedVarIncr(struct _attribute attr);
struct _attribute prefixedVarDecr(struct _attribute attr);

struct _attribute neg(struct _attribute);

//affiche la ligne d'operation en llvm
// void binOp(struct _variable*,struct _variable* , char* iOp, char* fOp, char* vOp);


struct _attribute multiply(struct _attribute ,struct _attribute);
struct _attribute divide(struct _attribute ,struct _attribute);
struct _attribute add(struct _attribute ,struct _attribute);
struct _attribute sub(struct _attribute ,struct _attribute);


struct _attribute l_op (struct _attribute,struct _attribute);
struct _attribute g_op (struct _attribute,struct _attribute);
struct _attribute le_op (struct _attribute,struct _attribute);
struct _attribute ge_op (struct _attribute,struct _attribute);
struct _attribute ne_op (struct _attribute,struct _attribute);
struct _attribute eq_op (struct _attribute,struct _attribute);



void affectValue (struct _attribute,enum _affectation ,struct _attribute);
struct _variable * declareVar(char* nom, struct _node* htab);
struct _attribute getVar(char* nom,struct _node* htab);


void setType(struct _variable *var, enum _type t);
void setTypeList(struct _list * list, enum _type t);



#endif //FUNCTIONS_H
