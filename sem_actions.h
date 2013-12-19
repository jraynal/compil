#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

struct _variable *mul(struct _variable *,struct _variable *);
struct _variable *div(struct _variable *,struct _variable *);
struct _variable *add(struct _variable *,struct _variable *);
struct _variable *sub(struct _variable *,struct _variable *);


struct _variable *incr(struct _variable *);
struct _variable *decr(struct _variable *);
struct _variable *neg(struct _variable *);







#endif //FUNCTIONS_H