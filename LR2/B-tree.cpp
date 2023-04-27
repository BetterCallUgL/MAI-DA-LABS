#include "B-tree.h"

#include <iostream>

#include "string.h"

bool More(char* a, char* b) {
    if (strlen(a) > strlen(b)) {
        return true;
    } else if (strlen(b) > strlen(a)) {
        return false;
    }
    for (int i = 0; i < strlen(a); i++) {
        if (int(a[i]) > int(b[i])) {
            return true;
        } else if (int(b[i]) > int(a[i])) {
            return false;
        }
    }
    return false;
}

void InitTree(Btree* tree, int number) {
    tree->t = number;
    tree->root = (node*)malloc(sizeof(node));
    tree->root->size = 0;
    tree->root->is_leaf = true;
    tree->root->values = (unsigned long long*)malloc(sizeof(unsigned long long) * (2 * number - 1));
    tree->root->keys = (char**)malloc(sizeof(char*) * (2 * number - 1));
    tree->root->childs = (node**)malloc(sizeof(node*) * 2 * number);
}

void NodeSplit(node* parent, int t, int index) {
    printf("сплит начался\n");
    node* right_child = (node*)malloc(sizeof(node));
    right_child->childs = (node**)malloc(sizeof(node*) * 2 * t);
    right_child->values = (unsigned long long*)malloc(sizeof(unsigned long long) * (2 * t - 1));
    right_child->keys = (char**)malloc(sizeof(char*) * (2 * t - 1));
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
    for (int j = parent->size; j >= index + 1; j--) {
        parent->childs[j + 1] = parent->childs[j];
    }
    parent->childs[index + 1] = right_child;
    for (int j = parent->size - 1; j >= index; j--) {
        parent->keys[j + 1] = parent->keys[j];
        parent->values[j + 1] = parent->values[j];
    }
    parent->keys[index] = left_child->keys[t - 1];
    parent->values[index] = left_child->values[t - 1];
    parent->size += 1;
    printf("Сплит закончен\n");
}

void FreeInsert(node* current_node, char* key, unsigned long long value, int t) {
    int index = current_node->size - 1;
    printf("вставка %s\n", key);
    if (current_node->is_leaf) {  // если лист
        printf("это лист\n");
        while (index >= 0 && More(current_node->keys[index], key)) {
            current_node->keys[index + 1] = current_node->keys[index];
            current_node->values[index + 1] = current_node->values[index];
            index -= 1;
        }
        current_node->keys[index + 1] =
            (char*)malloc(sizeof(char) * strlen(key) + 1);  // АХХАХАХАХАХАХАХХАХАХАФХАХФВХАВЫФХАХЫАХАХХАХАХ
        strncpy(current_node->keys[index + 1], key, strlen(key));
        current_node->values[index + 1] = value;
        current_node->size += 1;
    } else {  // не лист
        printf("это не лист\n");
        while (index >= 0 && More(current_node->keys[index], key)) {
            index -= 1;
        }
        index += 1;
        printf("это не лист2\n");
        printf("%d\n", index);
        printf("%s\n", current_node->keys[0]);
        printf("-----------\n");
        if (current_node->childs[index]->size == 2 * t - 1) {
            printf("зашёл в условие");
            NodeSplit(current_node, t, index);
            if (More(key, current_node->keys[index])) {
                index += 1;
            }
        }
        // printf("это не лист3\n");
        FreeInsert(current_node->childs[index], key, value, t);
    }
}

unsigned long long search(node* root, char* key) {
    if (root->is_leaf == false && More(root->keys[0], key)) {
        printf("я самый младший\n");
        return search(root->childs[0], key);
    } else if (root->is_leaf == false && More(key, root->keys[root->size - 1])) {
        printf("самый старший\n");
        return search(root->childs[root->size], key);
    } else {
        printf("я посередине\n");
        for (int i = 0; i < root->size; ++i) {
            if (!strcmp(key, root->keys[i])) {
                return root->values[i];
            }
            if (root->is_leaf == false && root->size > 1 && More(key, root->keys[i]) &&
                More(root->keys[i + 1], key)) {
                return search(root->childs[i + 1], key);
            }
        }
    }
    return -1;
}

void NodeInsert(Btree* tree, char* key, unsigned long long value) {
    node* old_root = tree->root;
    if (old_root->size == 2 * (tree->t) - 1) {  // увеличиваем высоту
        node* new_root = (node*)malloc(sizeof(node));
        new_root->is_leaf = false;
        new_root->childs = (node**)malloc(sizeof(node*) * 2 * tree->t);
        new_root->values = (unsigned long long*)malloc(sizeof(unsigned long long) * (2 * tree->t - 1));
        new_root->keys = (char**)malloc(sizeof(char*) * (2 * tree->t - 1));
        new_root->size = 0;
        new_root->childs[0] = old_root;
        tree->root = new_root;
        NodeSplit(new_root, tree->t, 0);
        FreeInsert(new_root, key, value, tree->t);
    } else {
        FreeInsert(old_root, key, value, tree->t);
    }
}


// удаление:
void DeleteLeaf(node* current_node, int index){
    for (int i = index + 1; i < current_node->size;++i){
        current_node->keys[i - 1] = current_node->keys[i];
        current_node->values[i - 1] = current_node->values[i]; 
    }
    current_node->size--;
}

char* TheBiggest(node* current_node, int index){
    node* tmp = current_node->childs[index];
    while (tmp->is_leaf == false){
        tmp = tmp->childs[tmp->size];
    }

    return tmp->keys[tmp->size - 1];
}

char* TheSmallest(node* current_node, int index){
    node* tmp = current_node->childs[index + 1];
    while(tmp->is_leaf == false){
        tmp = tmp->childs[0];
    }
    return tmp->keys[0];
}

void merge(node* current_node,void){

}

void DeleteNotLeaf(node* current_node, int index, int t){
    char* key = current_node->keys[index];

    // если у кого-то из детей > t - 1
    if (current_node->childs[index]->size > t - 1) {
        char* biggest_key = TheBiggest(current_node, index);
        strncpy(current_node->keys[index],biggest_key,strlen(biggest_key));
        Delete(current_node->childs[index],biggest_key,t);

    } else if (current_node->childs[index + 1]->size > t - 1){
        char* smallest_key = TheSmallest(current_node,index);
        strncpy(current_node->keys[index],smallest_key,strlen(smallest_key));
        Delete(current_node->childs[index], smallest_key,t);

    } else {
        Merge(current_node,index,t);
        remove(current_node->childs[index],key,t);
    }
}



void Delete(node* root, char* key, int t) {
    bool in_node = false;
    int node_index;
    for (int index = 0; index < root->size; ++index) {
        if (!strcmp(root->keys[index], key)) {
            in_node = true;
            node_index = index;
        }
    }
    if (in_node){
        if (root->is_leaf){
            DeleteLeaf(root, node_index);
        } else {
            DeleteNotLeaf(root, node_index, t);
        }
    } else {
        
    }
}