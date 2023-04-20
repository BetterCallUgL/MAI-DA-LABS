#include "B-tree.h"

#include <iostream>

void InitTree(Btree* tree, int number) {
    tree->t = number;
    tree->root->size = 0;
    tree->root->is_leaf = true;
    tree->root->values = (unsigned long long*)malloc(sizeof(unsigned long long) * (2 * number - 1));
    tree->root->keys = (char**)malloc(sizeof(char*) * (2 * number - 1));
    tree->root->childs = NULL;
}

node* NodeInsert(Btree* tree, char** key, double value) {
    node* old_root = tree->root;
    if (old_root->size == 2 * (tree->t) - 1) {
        node* new_root = (node*)malloc(sizeof(node));
        new_root->is_leaf = false;
        new_root->childs = (node**)malloc(sizeof(node*) * 2 * tree->t);
        new_root->values = (unsigned long long*)malloc(sizeof(unsigned long long) * (2 * tree->t - 1));
        new_root->keys = (char**)malloc(sizeof(char*) * (2 * tree->t - 1));
        new_root->size = 0;
        new_root->childs[0] = old_root;
        tree->root = new_root;
        NodeSplit(new_root, tree->t, 0);
        return FreeInsert(new_root,key,value,0);
    } else {
        return FreeInsert(old_root,key,value,tree->t);
    }
}



void NodeSplit(node* parent, int t, int index) {
    node* right_child = (node*)malloc(sizeof(node*));
    node* left_child = parent->childs[index];
    right_child->is_leaf = left_child->is_leaf;
    left_child->size = t - 1;
    right_child->size = t - 1;
    for (int i = 0; i < t - 1; ++i) {
        right_child->keys[i] = left_child->keys[i + t];
        right_child->values[i] = left_child->values[i + t];
    }
    if (left_child->is_leaf == false) {
        for (int i = 0; i < t; ++i) {
            right_child->childs[i] = left_child->childs[i + t];
        }
    }
    //доделоть.
}
