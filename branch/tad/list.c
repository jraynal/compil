#include "list.h"

int is_empty(struct _list * l){
	return l==NULL || l->head == NULL || l->tail==NULL || l->size<=0;
}

struct _nodeList * createnode(){
	struct _nodeList * node = malloc(sizeof(struct _nodeList *));
	if (node ==NULL){
		fprintf(stderr, "Allocation failed (%s:%d)\n",__FILE__,__LINE__);
		return NULL;
	}
	node->value =NULL;
	node->prev=NULL;
	node->next=NULL;
	return node;
}

struct _list * init_list(){
	struct _list * list = malloc(sizeof(struct _list *));
	if (list ==NULL){
		fprintf(stderr, "Allocation failed (%s:%d)\n",__FILE__,__LINE__);
		return NULL;
	}
	list->size=0;
	list->head=NULL;
	list->tail=NULL;
	return list;
}

int del_list(struct _list * l){
	if (l ==NULL){
		fprintf(stderr, "ERROR No list passed in arguments\n" );
		return EXIT_FAILURE;
	}
	while(! is_empty (l)){
		removeElmnt(l->head->value,l);
	}
	free(l);
	return EXIT_SUCCESS;
}



int insertElmnt(void* val,struct _list * l){
	if (l ==NULL){
		fprintf(stderr, "ERROR No list passed in arguments\n" );
		return EXIT_FAILURE;
	}
	struct _nodeList * node= createnode();
	if(node == NULL)
		return EXIT_FAILURE;
	node->value = val;
	node->next= l->head;
	l->head =node;
	l->size ++;
	return EXIT_SUCCESS;
}

struct _nodeList * find(void* elem,struct _list * l){
	if (elem ==NULL || l == NULL){
		fprintf(stderr, "ERROR(%s,%d)Invalid arguments\n",__FILE__,__LINE__);
		return NULL;
	}
	if (is_empty(l))
		return NULL;

	struct _nodeList * current = l->head;
	while(current->value != elem){
		//on cherche le premier element qui contient la valeur cherchée
		current= current->next;
		//si on est en fin de la liste (elem non trouvé)
		if (current ==NULL)
			return NULL;
	}
	return current;
}

int removeElmnt(void* elem,struct _list * l){
	if (is_empty(l)){
		fprintf(stderr, "ERROR No list passed in arguments\n" );
		return EXIT_FAILURE;
	}
	if (elem==NULL){
		fprintf(stderr, "ERROR It is not un acceptable value for a list content\n");
		return EXIT_FAILURE;
	}

	struct _nodeList * result = find (elem,l);
	if (result ==NULL)
		//elem pas dans l
		return EXIT_SUCCESS;
	else if (l->size <=1){
		//l'unique element est celui recherché
		free (result);
		l->head = NULL;
		l->tail = NULL;
	}else if(result->prev ==NULL){
		//si en tete de liste
		result->next->prev=NULL;
		l->head = result->next;
		free(result);
	}else if (result->next==NULL){
		//si en queue de liste
		result->prev->next=NULL;
		l->tail=result->prev;
	}else{
		//si au milieu
		result->prev->next=result->next;
		result->next->prev=result->prev;
	}
	l->size --;
	return EXIT_SUCCESS;
}



