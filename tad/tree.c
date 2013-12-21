#include "tree.h"
#include <stdio.h>

static int is_leaf(struct _node* node);
static int is_last_son(struct _node* node);
static int add_node(struct _node* father, char c);

struct _node *init_tree(){
	struct _node *root =(struct _node *)malloc(sizeof(struct _node));
	root->c='/';
	root->variable=NULL;
	root->first_son=NULL;
	root->brother=NULL;
	root->last_son=NULL;
	return root;	
}
int del_bro(struct _node *node) {
	if(node==NULL)
		return EXIT_FAILURE;
	fprintf(stderr,"%c\n",node->c);
	if(!is_last_son(node))
		del_bro(node->brother);
	if(node->variable!=NULL)
		free(node->variable);
	free(node);
	return EXIT_SUCCESS;
}
int del_tree(struct _node *root){
	/* Deja free ou blague... */
	if(root==NULL)
		return EXIT_FAILURE;
	/* Si on est sur un feuille c'est facile! */
	if(!is_leaf(root))
		del_tree(root->first_son);
	del_bro(root);
	return EXIT_SUCCESS;
}

int set_node(struct _node *root, char *path, struct _variable *data){
	struct _node *current = root;
	int i=0,exit;
	if(root==NULL||path==NULL)
		return EXIT_FAILURE;
	while(path[i]!='\0'){
		exit=1;
		/* On verifie que le noeud n'existe pas déjà */
		do{
			if(current->c==path[i]){
				/* Un noeud possède la lettre, on avance */
				exit=0;
				i++;
			}
			else if(is_last_son(current)){
				/* Si la lettre n'est pas dans la liste des fils */
				add_node(current,path[i]);
				i++;
			}
			if(path[i]=='\0'){
				/* On est à la fin du mot, on remplace les données */
				//if(current->variable!=NULL)
				//	free(current->variable);
				current->variable=data;
			}
		/* controleur de boucle */
		if(is_last_son(current))
			exit=0;
		else
			current=current->brother;
		}while(exit);
		/* On ajoute le suivant si on est sur une feuille*/
		if(is_leaf(current))
			add_node(current,path[i]);
		current=current->first_son;
	}
	return EXIT_SUCCESS;
}

struct _variable *get_node(struct _node *root, char *path){
	if(root == NULL){
		fprintf(stderr,"ERROR (%s:%d): Not valid tree ",__FILE__,__LINE__);
		return NULL;

	}

	if(root==NULL||path==NULL){
		return NULL;
	}
	struct _node *father = root;
	struct _node *current = root->first_son;
	int i=1,exit=0; // i commence à 1 pour éviter le "/"
	while(path[i]!='\0'){
		if(current==NULL)
			return NULL;
		exit=1;
		do{
			/* On cherche la lettre dans les fils */
			if(current->c==path[i]){
				/* Le noeud devient le nouveau père */
				father=current;
				i++;
				exit=0;
			}
			/* Si la lettre n'existe pas, on jette */
			else if(is_last_son(current)){
				return NULL;
			}
			/* sinon on itère */
			else
				current=current->brother;
			/* Si on arrive à la fin du chemin, on renvoie la variable */
			if(path[i]=='\0')
				return current->variable;
		}while(exit);
		current=father->first_son;
	}
	return NULL;
}

// private
static int is_leaf(struct _node *node){
	if(node==NULL)
		return EXIT_FAILURE;
	else if((node->first_son==NULL)&&(node->last_son==NULL))
		return 1;
	else
		return 0;
}

static int is_last_son(struct _node *node){
	if(node==NULL)
		return EXIT_FAILURE;
	else if(node->brother==NULL)
		return 1;
	else
		return 0;
}

static int add_node(struct _node* father, char letter){
	if(father==NULL||letter=='\0')
		return EXIT_FAILURE;
	struct _node *node = init_tree();
	struct _node *penultimate = father->last_son;
	node->c=letter;
	if(penultimate==NULL)
		father->first_son=node;
	else
		penultimate->brother=node;
	father->last_son=node;
	return EXIT_SUCCESS;
}
