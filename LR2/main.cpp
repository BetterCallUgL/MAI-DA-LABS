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
            if (search(tree->root, key) != -1) {
                printf("Exist\n");
            } else {
                NodeInsert(tree, key, value);
                printf("OK\n");
            }
        }
        if (!strcmp(buffer, "print")) {
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
        }
    }
    return 0;
}