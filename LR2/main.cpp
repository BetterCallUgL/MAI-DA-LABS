#include <string.h>

#include <iostream>

#include "B-tree.h"

using namespace std;

int main() {
    const int number = 3;
    Btree* tree = (Btree*)malloc(sizeof(Btree));
    InitTree(tree,number);
    char buffer[258];
    while (cin >> buffer) {
        if (!strcmp(buffer, "+")) {
            char message[256];
            long long value;
            if (AddElement(root, ) == NULL) {
            }
        }
    }
    free(root);
    return 0;
}