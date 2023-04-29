#ifndef B_TREE_H
#define B_TREE_H

const int SIZE = 257;

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
unsigned long long Search(node* root, char* key);
void Delete(node* root, char* key, int t);
void Fill(node* current_node,int index,int t);
void Merge(node* current_node, int index, int t);
void DeleteLeaf(node* current_node, int index);
char* TheBiggest(node* current_node, int index);
char* TheSmallest(node* current_node, int index);

#endif