#include <string.h>

#include <iostream>

#include "B-tree.h"

const int number = 2;
using namespace std;

int main() {
    Btree* tree = (Btree*)malloc(sizeof(Btree));
    InitTree(tree, number);
    char buffer[258];
    while (cin >> buffer) {
        if (!strcmp(buffer, "+")) {
            char key[256];
            unsigned long long value;
            cin >> key >> value;
            for (int i = 0; i < strlen(key); i++) {
                key[i] = tolower(key[i]);
            }
            if (Search(tree->root, key) != 0) {
                printf("Exist\n");
            } else {
                NodeInsert(tree, key, value);
                printf("OK\n");
            }

        } else if (!strcmp(buffer, "print")) {
            for (int j = 0; j < tree->root->childs[0]->size; j++) {
                printf("\t%s %lld\n", tree->root->childs[0]->keys[j], tree->root->childs[0]->values[j]);
            }
            for (int i = 0; i < tree->root->size; i++) {
                printf("%s %lld\n", tree->root->keys[i], tree->root->values[i]);
                for (int j = 0; j < tree->root->childs[i + 1]->size; j++) {
                    printf("\t%s %lld\n", tree->root->childs[i + 1]->keys[j],
                           tree->root->childs[i + 1]->values[j]);
                }
            }
        }else if (!strcmp(buffer,"-")) {
            char key[256];
            cin >> key;
            for (int i = 0; i < strlen(key); i++) {
                key[i] = tolower(key[i]);
            }
            if (Search(tree->root,key) == 0){
                printf("NoSuchWord\n");
            } else {
                Delete(tree->root,key,tree->t);
            }
        } else {
            unsigned long long value = Search(tree->root, buffer);
            if (value == 0){
                printf("NoSuchWord\n");
            } else {
                printf("Ok: %lld\n",value);
            }
        }
    }
    return 0;
}