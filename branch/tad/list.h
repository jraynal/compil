#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>
#include "structs.h"

struct _list * init_list();
int del_list(struct _list * l);

int insertElmnt(void* val,struct _list * l);
int removeElmnt(void* elem,struct _list * l);


#endif
