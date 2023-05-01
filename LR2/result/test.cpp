#include <string.h>

#include <iostream>

const int SIZE = 257;

typedef struct node {
    bool is_leaf;
    char** keys;
    unsigned long long* values;
    int size;
    node** childs;
} node;

typedef struct Btree {
    int t;
    node* root;
} Btree;

void NodeDelete(Btree* tree, char* key);
void InitTree(Btree* tree);
void NodeInsert(Btree* tree, char* key, unsigned long long value);
unsigned long long* Search(node* root, char* key);
void Delete(node* root, char* key, int t);
void Fill(node* current_node, int index, int t);
void Merge(node* current_node, int index, int t);
void DeleteLeaf(node* current_node, int index);
char* TheBiggest(node* current_node, int index);
char* TheSmallest(node* current_node, int index);
void Destroy(node* root);
void Print(node* current_node, int depth);
void Save(Btree* tree, char* path);
void NodeSave(node* current_node, FILE* bfile);

const int t_num = 3;
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    Btree* tree = (Btree*)malloc(sizeof(Btree));
    tree->root = NULL;
    tree->t = t_num;
    char buffer[258];
    while (cin >> buffer) {
        if (!strcmp(buffer, "+")) {
            char key[256];
            unsigned long long value;
            cin >> key >> value;
            for (int i = 0; i < strlen(key); i++) {
                key[i] = tolower(key[i]);
            }
            if (Search(tree->root, key) != NULL) {
                cout << "Exist\n";
            } else {
                NodeInsert(tree, key, value);
                cout << "OK\n";
            }

        } else if (!strcmp(buffer, "print")) {
            Print(tree->root, 0);
        } else if (!strcmp(buffer, "-")) {
            char key[256];
            cin >> key;
            for (int i = 0; i < strlen(key); i++) {
                key[i] = tolower(key[i]);
            }
            if (Search(tree->root, key) == NULL) {
                cout << "NoSuchWord\n";
            } else {
                NodeDelete(tree, key);
                cout << "OK\n";
            }
        } else {
            for (int i = 0; i < strlen(buffer); i++) {
                buffer[i] = tolower(buffer[i]);
            }
            unsigned long long* value = Search(tree->root, buffer);
            if (value == NULL) {
                cout << "NoSuchWord\n";
            } else {
                cout << "OK: " << *value << "\n";
            }
        }
    }

    Destroy(tree->root);
    free(tree);
    tree = NULL;
    return 0;
}

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

void InitTree(Btree* tree) {
    tree->root = (node*)malloc(sizeof(node));
    tree->root->size = 0;
    tree->root->is_leaf = true;
    tree->root->values = (unsigned long long*)malloc(sizeof(unsigned long long) * (2 * tree->t - 1));
    tree->root->keys = (char**)malloc(sizeof(char*) * (2 * tree->t - 1));
    tree->root->childs = (node**)malloc(sizeof(node*) * 2 * tree->t);
}

void NodeSplit(node* parent, int t, int index) {
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
}

void FreeInsert(node* current_node, char* key, unsigned long long value, int t) {
    int index = current_node->size - 1;
    if (current_node->is_leaf) {  // если лист
        while (index >= 0 && More(current_node->keys[index], key)) {
            current_node->keys[index + 1] = current_node->keys[index];
            current_node->values[index + 1] = current_node->values[index];
            index -= 1;
        }
        current_node->keys[index + 1] = (char*)malloc(sizeof(char) * SIZE);
        strcpy(current_node->keys[index + 1], key);
        current_node->keys[index + 1][strlen(key)] = '\0';  // test
        current_node->values[index + 1] = value;
        current_node->size += 1;
    } else {  // не лист
        while (index >= 0 && More(current_node->keys[index], key)) {
            index -= 1;
        }
        index += 1;
        if (current_node->childs[index]->size == 2 * t - 1) {
            NodeSplit(current_node, t, index);
            if (More(key, current_node->keys[index])) {
                index += 1;
            }
        }
        FreeInsert(current_node->childs[index], key, value, t);
    }
}

unsigned long long* Search(node* root, char* key) {
    if (root == NULL) {
        return NULL;
    }
    if (root->is_leaf == false && More(root->keys[0], key)) {
        return Search(root->childs[0], key);
    } else if (root->is_leaf == false && More(key, root->keys[root->size - 1])) {
        return Search(root->childs[root->size], key);
    } else {
        for (int i = 0; i < root->size; ++i) {
            if (!strcmp(key, root->keys[i])) {
                return root->values + i;
            }
            if (root->is_leaf == false && root->size > 1 && More(key, root->keys[i]) &&
                More(root->keys[i + 1], key)) {
                return Search(root->childs[i + 1], key);
            }
        }
    }
    return NULL;
}

void NodeInsert(Btree* tree, char* key, unsigned long long value) {
    if (tree->root == NULL) {
        InitTree(tree);
    }
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
void DeleteLeaf(node* current_node, int index) {
    free(current_node->keys[index]);
    for (int i = index + 1; i < current_node->size; ++i) {
        current_node->keys[i - 1] = current_node->keys[i];
        current_node->values[i - 1] = current_node->values[i];
    }
    current_node->size--;
}

char* TheBiggest(node* current_node, int index) {
    node* tmp = current_node->childs[index];
    while (tmp->is_leaf == false) {
        tmp = tmp->childs[tmp->size];
    }

    return tmp->keys[tmp->size - 1];
}

char* TheSmallest(node* current_node, int index) {
    node* tmp = current_node->childs[index + 1];
    while (tmp->is_leaf == false) {
        tmp = tmp->childs[0];
    }
    return tmp->keys[0];
}

void Merge(node* current_node, int index, int t) {
    node* left_child = current_node->childs[index];
    node* right_child = current_node->childs[index + 1];

    left_child->keys[t - 1] = current_node->keys[index];
    left_child->values[t - 1] = current_node->values[index];

    for (int i = 0; i < right_child->size; ++i) {
        left_child->keys[i + t] = right_child->keys[i];
        left_child->values[i + t] = right_child->values[i];
    }

    if (left_child->is_leaf == false) {
        for (int i = 0; i <= right_child->size; i++) {
            left_child->childs[i + t] = right_child->childs[i];
        }
    }

    for (int i = index + 1; i < current_node->size; ++i) {
        current_node->keys[i - 1] = current_node->keys[i];
        current_node->values[i - 1] = current_node->values[i];
    }

    for (int i = index + 2; i <= current_node->size; ++i) {
        current_node->childs[i - 1] = current_node->childs[i];
    }
    current_node->size--;
    left_child->size += right_child->size + 1;
    if (right_child->is_leaf == false) {
        free(right_child->childs);
    }
    free(right_child->keys);
    free(right_child->values);
    free(right_child);
}

void DeleteNotLeaf(node* current_node, int index, int t) {
    char* key = current_node->keys[index];
    // если у кого-то из детей > t - 1
    if (current_node->childs[index]->size > t - 1) {
        char* biggest_key = TheBiggest(current_node, index);
        current_node->values[index] = *Search(current_node, biggest_key);
        strcpy(current_node->keys[index], biggest_key);
        current_node->keys[index][strlen(biggest_key)] = '\0';  // test
        Delete(current_node->childs[index], current_node->keys[index], t);

    } else if (current_node->childs[index + 1]->size > t - 1) {
        char* smallest_key = TheSmallest(current_node, index);
        current_node->values[index] = *Search(current_node, smallest_key);
        strcpy(current_node->keys[index], smallest_key);
        current_node->keys[index][strlen(smallest_key)] = '\0';  // test
        Delete(current_node->childs[index + 1], current_node->keys[index], t);

    } else {
        Merge(current_node, index, t);
        Delete(current_node->childs[index], key, t);
    }
}

void LeftSteal(node* current_node, int index) {
    node* child_to_put = current_node->childs[index];
    node* child_to_steal = current_node->childs[index - 1];

    for (int i = child_to_put->size - 1; i >= 0; --i) {
        child_to_put->keys[i + 1] = child_to_put->keys[i];
        child_to_put->values[i + 1] = child_to_put->values[i];
    }

    if (child_to_put->is_leaf == false) {
        for (int i = child_to_put->size; i >= 0; --i) {
            child_to_put->childs[i + 1] = child_to_put->childs[i];
        }
    }

    child_to_put->keys[0] = current_node->keys[index - 1];
    child_to_put->values[0] = current_node->values[index - 1];

    if (child_to_put->is_leaf == false) {
        child_to_put->childs[0] = child_to_steal->childs[child_to_steal->size];
    }

    current_node->keys[index - 1] = child_to_steal->keys[child_to_steal->size - 1];
    current_node->values[index - 1] = child_to_steal->values[child_to_steal->size - 1];

    child_to_put->size += 1;
    child_to_steal->size -= 1;
}

void RightSteal(node* current_node, int index) {
    node* child_to_put = current_node->childs[index];
    node* child_to_steal = current_node->childs[index + 1];

    child_to_put->keys[child_to_put->size] = current_node->keys[index];
    child_to_put->values[child_to_put->size] = current_node->values[index];

    if (child_to_put->is_leaf == false) {
        child_to_put->childs[child_to_put->size + 1] = child_to_steal->childs[0];
    }

    current_node->keys[index] = child_to_steal->keys[0];
    current_node->values[index] = child_to_steal->values[0];

    for (int i = 1; i < child_to_steal->size; ++i) {
        child_to_steal->keys[i - 1] = child_to_steal->keys[i];
        child_to_steal->values[i - 1] = child_to_steal->values[i];
    }

    if (child_to_steal->is_leaf == false) {
        for (int i = 1; i <= child_to_steal->size; ++i) {
            child_to_steal->childs[i - 1] = child_to_steal->childs[i];
        }
    }

    child_to_steal->size -= 1;
    child_to_put->size += 1;
}

void Fill(node* current_node, int index, int t) {
    if (index != 0 && current_node->childs[index - 1]->size > t - 1) {
        LeftSteal(current_node, index);
    } else if (index != current_node->size && current_node->childs[index + 1]->size > t - 1) {
        RightSteal(current_node, index);
    } else {
        if (index != current_node->size) {
            Merge(current_node, index, t);
        } else {
            Merge(current_node, index - 1, t);
        }
    }
}

void Delete(node* root, char* key, int t) {
    int node_index = 0;
    while (node_index < root->size && More(key, root->keys[node_index])) {
        ++node_index;
    }
    if (node_index < root->size && !strcmp(root->keys[node_index], key)) {
        if (root->is_leaf) {
            DeleteLeaf(root, node_index);
        } else {
            DeleteNotLeaf(root, node_index, t);
        }
    } else {
        if (root->childs[node_index]->size == t - 1) {
            Fill(root, node_index, t);
        }
        if (node_index > root->size) {
            Delete(root->childs[node_index - 1], key, t);
        } else {
            Delete(root->childs[node_index], key, t);
        }
    }
}

void NodeDelete(Btree* tree, char* key) {
    if (tree->root != NULL) {
        Delete(tree->root, key, tree->t);
        if (tree->root->size == 0) {
            node* tmp = tree->root;
            if (tree->root->is_leaf) {
                tree->root = NULL;
            } else {
                tree->root = tree->root->childs[0];
            }
        }
    }
}

void Destroy(node* root) {
    if (root != NULL) {
        free(root->values);
        for (int index = 0; index < root->size; ++index) {
            free(root->keys[index]);
        }
        if (root->is_leaf == false) {
            for (int index = 0; index <= root->size; ++index) {
                Destroy(root->childs[index]);
            }
        }

        free(root->keys);
        free(root->childs);
        free(root);
    }
}

void Print(node* current_node, int depth) {
    if (current_node == nullptr) {
        cout << "Empty";
        return;
    }
    for (int i = 0; i < current_node->size; ++i) {
        if (current_node->is_leaf == false) {
            Print(current_node->childs[i], depth + 1);
        }
        for (int j = 0; j < depth; ++j) {
            cout << "\t";
        }
        cout << current_node->keys[i] << " " << current_node->values[i] << "\n";
    }

    if (current_node->is_leaf == false) {
        Print(current_node->childs[current_node->size], depth + 1);
    }
}

void Save(Btree* tree, char* path) {
    FILE* bfile = fopen(path, "wb");
    if (tree->root != NULL) {
        NodeSave(tree->root, bfile);
    }
    char* end_char = "*";
    fwrite(end_char, 1, 1, bfile);
}

void NodeSave(node* current_node, FILE* bfile) {
    if (current_node->is_leaf == false){
        for (int index = 0; index <= current_node->size; ++index) { 
            NodeSave(current_node->childs[index],bfile);
        }
    }

    for (int index = 0; index < current_node->size; ++ index){
        char number[21];
        sprintf(number,"%d",current_node->values[index]);
        
    }
}