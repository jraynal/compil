#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tad/tree.h"
#include "tad/list.h"
#include "tad/structs.h"
#include "tad/layer.h"
#include "tad/code.h"
#include "tad/int_heap.h"

#define LOG(message) do{fprintf(stderr,"[LOG][%s]: "#message" at\t%s\tin %s\t\t\tline\t\t%d\n",__TIME__,__FILE__,__func__,__LINE__);}while(0);
 //for dynamic allocation

struct _variable * varCreate(enum _type type, const char * addr);
// struct _attribute *varCreateInt(int );
// struct _attribute *varCreateFloat(float );

struct _attribute *getVar(const char* nom,struct _layer* htab);
struct _attribute *newInt(int i);
struct _attribute *newFloat(float f);
struct _attribute *simpleFuncall(struct _layer* htab,const char * funName);
struct _attribute *multipleFuncall(struct _layer* htab,const char * funName,struct _list * l);
struct _attribute *varIncr(const char * name,struct _layer* htab);
struct _attribute *varDecr(const char * name,struct _layer* htab);

struct _attribute *getValArray(struct _attribute *array, struct _attribute *i);

struct _list * expression_list(struct _attribute*);
struct _list * insert_expr_list(struct _attribute *a ,struct _list * l);

struct _attribute *prefixedVarIncr(struct _attribute *attr);
struct _attribute *prefixedVarDecr(struct _attribute *attr);

struct _attribute *neg(struct _attribute*);

//affiche la ligne d'operation en llvm
// void binOp(struct _variable*,struct _variable* , char* iOp, char* fOp, char* vOp);


struct _attribute *multiply(struct _attribute *,struct _attribute *);
struct _attribute *divide(struct _attribute *,struct _attribute *);
struct _attribute *add(struct _attribute *,struct _attribute *);
struct _attribute *sub(struct _attribute *,struct _attribute *);


struct _attribute *l_op (struct _attribute *,struct _attribute *);
struct _attribute *g_op (struct _attribute *,struct _attribute *);
struct _attribute *le_op (struct _attribute *,struct _attribute *);
struct _attribute *ge_op (struct _attribute *,struct _attribute *);
struct _attribute *ne_op (struct _attribute *,struct _attribute *);
struct _attribute *eq_op (struct _attribute *,struct _attribute *);

struct _attribute *declareVar(char* nom);

struct _attribute *declare_array(struct _attribute* array, int size);

struct _attribute *simple_declare_function(struct _attribute * func);
struct _attribute *multiple_declare_function(struct _attribute * func , struct _list * args);

struct _attribute *allocate_id(struct _layer *,struct _attribute *,enum _type);
void setType(struct _attribute *a, enum _type t);
struct _attribute * setTypeList(struct _list * list, enum _type t);
struct _attribute *make_function(enum _type t , struct _attribute * name, struct _attribute * content);

struct _attribute * emptyExpr();

void print(struct _attribute *a);
struct _attribute *selection(struct _attribute *, struct _attribute *, struct _attribute *);
struct _attribute *loop(struct _attribute *,struct _attribute *,struct _attribute *, struct _attribute *);

struct _attribute *concat(struct _attribute *a1, struct _attribute *a2);
struct _attribute *assignment(struct _attribute *tgt, enum _affectation ,struct _attribute *ori);

#endif //FUNCTIONS_H
