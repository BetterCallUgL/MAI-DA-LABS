#ifndef B_TREE_H
#define B_TREE_H


typedef struct node {
    bool is_leaf;
    char** keys;
    unsigned long long* values;
    int size;
    node** childs;
} node;

typedef struct Btree{
    int t;
    node* root;
}Btree;

void InitTree(Btree* tree, int number);
void NodeInsert(Btree* tree, char* key, unsigned long long value);
unsigned long long search(node* root, char* key);

#endif