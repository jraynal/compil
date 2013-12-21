#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <stdio.h>
#include "structs.h"

struct _node * init_tree();
int del_tree(struct _node *root);
int set_node(struct _node *root, char *path, struct _variable *data);
struct _variable *get_node(struct _node *root, char *path);

#endif
