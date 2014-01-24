#include "list.h"

#define CHK(truc) do{if(truc == NULL){printf("%s est NULL\n",#truc); exit(1);}}while(0)


int is_valid(struct _list* l){
	//l is not null and head and tail are both affected or not)
	return (l != NULL) 
			&&  ( (l->head && l->tail) || (! l->head && ! l->tail));

}

int is_empty(struct _list * l){
	if (! is_valid(l))
		exit(1);
	return l->head == NULL || l->tail==NULL || l->size<=0;
}

struct _nodeList * createnode(){
	struct _nodeList * node = malloc(sizeof(struct _nodeList));
	CHK(node);
	node->value =NULL;
	node->prev=NULL;
	node->next=NULL;
	return node;
}

struct _list * init_list(){
	struct _list * list = malloc(sizeof(struct _list));
	CHK(list);
	list->size=0;
	list->head=NULL;
	list->tail=NULL;
	return list;
}

int del_list(struct _list * list){
	CHK(list);
	if(!is_valid(list))
		exit(1);
	while(! is_empty (list)){
		removeElmnt(list->head->value,list);
	}
	free(list);
	return EXIT_SUCCESS;
}


int del_list_and_content(struct _list * l){
	if (l ==NULL){
		fprintf(stderr, "ERROR No list passed in arguments\n" );
		return EXIT_FAILURE;
	}
	while(! is_empty (l)){
		struct _variable * var = l->head->value;
		removeElmnt(var,l);
		free(var);
	}
	free(l);
	return EXIT_SUCCESS;
}

struct _nodeList * find(void* val,struct _list * list){
	CHK(list);
	CHK(val);
	if(! is_valid(list))
		exit(1);

	if (is_empty(list))
		return NULL;

	struct _nodeList * current = list->head;
	while(current != NULL && current->value != val){
		//on cherche le premier element qui contient la valeur cherchée
		current= current->next;
	}
	return current; // vaut NULL si en bas de list
}


int insertElmnt(void* val,struct _list * list){
	CHK(list);
	if(! is_valid(list))
		exit(1);
	if(find(val,list) != NULL)
		return EXIT_SUCCESS;

	struct _nodeList * node= createnode();
	if(node == NULL)
		return EXIT_FAILURE;
	node->value = val;
	node->next= list->head;

	if(! is_empty(list))
		list->head->prev = node;

	list->head =node;
	if (list->tail == NULL)
		list->tail = node;
	list->size ++;
	return EXIT_SUCCESS;
}


int removeElmnt(void* val,struct _list * list){
	CHK(list);
	CHK(val);

	struct _nodeList * result = find (val,list);
	if (result ==NULL)
		//val pas dans list
		return EXIT_SUCCESS;
	else if (list->size <=1){
		//l'unique element est celui recherché
		list->head = NULL;
		list->tail = NULL;
	}else if(result->prev ==NULL){
		//si en tete de liste
		result->next->prev=NULL;
		list->head = result->next;
	}else if (result->next==NULL){
		//si en queue de liste
		result->prev->next=NULL;
		list->tail=result->prev;
	}else{
		//si au milieu
		result->prev->next=result->next;
		result->next->prev=result->prev;
	}
	free(result);
	if (list->size > 0)
		list->size --;
	return EXIT_SUCCESS;
}



