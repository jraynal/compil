#include "int_heap.h"

#define CHK(truc) do{if(truc == NULL) fprintf(stderr,#message" in "#truc" at %s in %s line %d\n",__FILE__,__func__,__LINE__);exit(EXIT_FAILURE);}while(0)

// INCLUDED TREE (self balancing tree where the root is always lower than the sons)

// Create the included tree
struct _self_balancing_tree * init_self_balancing_tree(){
	struct _self_balancing_tree * tree = malloc(sizeof(struct _self_balancing_tree));
	CHK(tree);
	tree-> weight =0;
	tree-> left =NULL;
	tree->right =NULL;
	return tree;
}

//free memory from the tree
int delete_self_balancing_tree(struct _self_balancing_tree * tree){
	CHK(tree);
	if (tree->left != NULL){
		delete_self_balancing_tree(tree->left);
		tree->left =NULL;
	}
	if (tree->right != NULL){
		delete_self_balancing_tree(tree->right);
		tree->right =NULL;
	}
	free(tree);
	tree = NULL;
	return(EXIT_SUCCESS);
}


// add an integer in the tree
int insert_int_heap(struct _self_balancing_tree * tree, int val){
	CHK(tree);

	//if already in the tree
	if(tree->value == val && tree->weight != 0)
		return EXIT_SUCCESS;
	
	(tree->weight)++;
	
	//if tree is empty
	if(tree->weight == 1){
		tree->value = val;
		return EXIT_SUCCESS;
	}

	int value_to_insert = val;
	// if the value of the node is greater than val, they are swaped : to keep realtion between the father and its sons
	if (tree->value > val){
		value_to_insert = tree->value;
		tree->value = val;
	}

	//if left is not declared
	if (tree->left == NULL){
		tree->left= init_self_balancing_tree();
		return insert_int_heap(tree->left,value_to_insert);
	}
	//if right is not declared
	if (tree->right == NULL){
		tree->right= init_self_balancing_tree();
		return insert_int_heap(tree->right,value_to_insert);
	}  
	// else insert in the lightest side
	if (tree->left->weight > tree->right->weight)
		return insert_int_heap(tree->right,value_to_insert);
	else
		return insert_int_heap(tree->left,value_to_insert);
	
}

// remove the integer from the tree
int get_int_tree(struct _self_balancing_tree* tree){
	CHK(tree);
	int ret=-1;
	if(tree->weight<= 0)
		return EXIT_FAILURE;

	(tree->weight)--;

	if (tree->weight == 0){
		return tree->value;
	}
	CHK(tree->left);
	if(tree->right == NULL || tree->left->weight >= tree->right->weight){
		ret  = get_int_tree(tree->left);
		if(tree->left->weight == 0){
			delete_self_balancing_tree(tree->left);
			tree->left = NULL;
		}
		return ret;
	}
	else{
		ret = get_int_tree(tree->right);
		if (tree->right->weight == 0){
			delete_self_balancing_tree(tree->right);
			tree->right =NULL;
		}
		return ret;
	}
}



// FULL HEAP

struct _int_heap * init_int_heap(){
	struct _int_heap * heap = malloc(sizeof(struct _int_heap));
	CHK(heap);
	heap->tree = init_self_balancing_tree();
	heap->first_free = 0;
	return heap;
}

int delete_int_heap(struct _int_heap* heap){
	CHK(heap);
	if (heap->tree != NULL)
		delete_self_balancing_tree(heap->tree);
	free(heap);
	heap = NULL;
	return EXIT_SUCCESS;
}

int get_free_int(struct _int_heap* heap){
	CHK(heap);
	CHK(heap->tree);
	if (heap->tree->weight <= 0)
		return (heap->first_free)++;
	else
		return get_int_tree(heap->tree);
}

int free_int(struct _int_heap* heap , int num){
	CHK(heap);
	CHK(heap->tree);
	insert_int_heap(heap->tree,num);
	return EXIT_SUCCESS;
}


// for debug
void print_tree(struct _self_balancing_tree* tree){
	CHK(tree);
	if(tree->weight > 0)
		printf("(\"%d\"{%d} | ",tree->value,tree->weight);
	if(tree->left != NULL)
		print_tree(tree->left);
	printf("|");
	if(tree->right != NULL)
		print_tree(tree->right);
	printf(")");
	
}


void print_content(struct _int_heap *heap){
	CHK(heap);
	CHK(heap->tree);
	print_tree(heap->tree);
	printf("\n");
}



