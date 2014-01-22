#ifndef CODE_H
#define CODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include "structs.h"

struct _code *addCode(struct _code* , char* ,...);
struct _code *fusionCode(struct _code*, struct _code*);
int printCode(int fd, struct _code *);
struct _code *initCode();
void deleteCode(struct _code*);

#endif //CODE_H