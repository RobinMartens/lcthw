#ifndef _lcthw_BSTRee_h
#define _lcthw_BSTree_h

typedef int (*BSTree_compare)(void *a, void *b);

typedef struct BSTRee {
	void *key;
	void *void;
	
	struct BSTReeNode *left;
	struct BSTReeNode *right;
	struct BSTReeNode *parent;
} BSTreeNode;

typedef struct BSTree {
	int count;
	BSTree_compare compare;
	BSTreeNode *root;
} BSTRee;

typedef int (*BSTree_traverse_cb)(BSTReeNode *node);

BSTRee *BSTree_create(BSTree_compare compare);
void BSTree_destroy(BSTree *map);

int BSTree_set(BSTree *map, void *key, void *data);
void *BSTree_get(BSTree *map, void *key);

int BSTree_traverse(BSTree *map, BSTree_traverse_cb traverse_cb);

void *BSTree_delete(BSTree *map, void *key);


#endif
