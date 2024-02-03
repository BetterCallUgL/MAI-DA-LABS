#include <math.h>
#include <string.h>

#include <chrono>
#include <iostream>

const int MAX_SIZE = 30;

typedef struct Answer {
  int* array;
  int size;
} Answer;

Answer* result = (Answer*)malloc(sizeof(Answer));

typedef struct Node {
  Node* children[MAX_SIZE];
  Node* suffix_link;
  int start;
  int* end;
  int suffix_index;
} Node;

typedef struct Point {
  Node* active_node;
  int active_length;
  int active_edge;
} Point;

typedef struct SuffixTree {
  char* text;
  Node* root;
  Node* latest_node;
  Point* active_point;
  int suffix_remaining;
  int leaf_end;
  int* root_end;
  int* split_end;
  int text_size;
} SuffixTree;

void BubbleSort(int* a, int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (a[j] > a[j + 1]) {
        int c = a[j];
        a[j] = a[j + 1];
        a[j + 1] = c;
      }
    }
  }
}

Node* newNode(SuffixTree* tree, int start, int* end) {
  Node* node = (Node*)malloc(sizeof(Node));
  for (int i = 0; i < MAX_SIZE; i++) {
    node->children[i] = NULL;
  }

  node->suffix_link = tree->root;
  node->start = start;
  node->end = end;
  node->suffix_index = -1;

  return node;
}

// WARNING!!! МОжет не работать.........
int SwitchActiveNode(SuffixTree* tree, Node* node) {
  int edge_length = *(node->end) - node->start + 1;
  if (tree->active_point->active_length >= edge_length) {
    tree->active_point->active_edge += edge_length;
    tree->active_point->active_length -= edge_length;
    tree->active_point->active_node = node;
    return 1;
  }
  return 0;
}

void addToTree(SuffixTree* tree, int index) {
  tree->leaf_end = index;
  tree->suffix_remaining += 1;
  tree->latest_node = NULL;

  while (tree->suffix_remaining > 0) {
    if (tree->active_point->active_length == 0) {
      tree->active_point->active_edge = index;
    }

    Node* next = tree->active_point->active_node->children[(
        int)(tree->text[tree->active_point->active_edge] - 'a')];
    // if such edge doesn't exists
    if (next == NULL) {
      tree->active_point->active_node
          ->children[(int)(tree->text[tree->active_point->active_edge] - 'a')] =
          newNode(tree, index, &(tree->leaf_end));

      if (tree->latest_node != NULL) {
        tree->latest_node->suffix_link = tree->active_point->active_node;
        tree->latest_node = NULL;
      }

      // else
    } else {
      if (SwitchActiveNode(tree, next)) {
        continue;
      }
      // such char already exists.
      if (tree->text[next->start + tree->active_point->active_length] ==
          tree->text[index]) {
        if (tree->latest_node != NULL &&
            tree->active_point->active_node != tree->root) {
          tree->latest_node->suffix_link = tree->active_point->active_node;
          tree->latest_node = NULL;
        }

        tree->active_point->active_length++;
        break;
      }

      // case we must split
      tree->split_end = (int*)malloc(sizeof(int));
      *(tree->split_end) = next->start + tree->active_point->active_length - 1;

      Node* new_node = newNode(tree, next->start, tree->split_end);
      tree->active_point->active_node
          ->children[(int)(tree->text[next->start] - 'a')] = new_node;

      new_node->children[(int)(tree->text[index] - 'a')] =
          newNode(tree, index, &(tree->leaf_end));
      next->start += tree->active_point->active_length;
      new_node->children[(int)(tree->text[next->start] - 'a')] = next;

      if (tree->latest_node != NULL) {
        tree->latest_node->suffix_link = new_node;
      }

      tree->latest_node = new_node;
    }

    tree->suffix_remaining--;
    if (tree->active_point->active_node == tree->root &&
        tree->active_point->active_length > 0) {
      tree->active_point->active_length--;
      tree->active_point->active_edge = index - tree->suffix_remaining + 1;
    }

    else if (tree->active_point->active_node != tree->root) {
      tree->active_point->active_node =
          tree->active_point->active_node->suffix_link;
    }
  }
}

void SetIndexOfSuffix(Node* src, int current_size, SuffixTree* tree) {
  if (src == NULL) {
    return;
  }
  // if (src->start != -1) {
  //   for (int i = src->start; i <= *(src->end); i++) {
  //     printf("%c", tree->text[i]);
  //   }
  // }
  int is_leaf = 1;
  for (int i = 0; i < MAX_SIZE; i++) {
    if (src->children[i] != NULL) {
      // if (is_leaf == 1 && src->start != -1) {
      //   printf(" [%d]\n", src->suffix_index);
      // }
      is_leaf = 0;

      int edge_length =
          *(src->children[i]->end) - (src->children[i]->start) + 1;
      SetIndexOfSuffix(src->children[i], current_size + edge_length, tree);
    }
  }
  if (is_leaf == 1) {
    src->suffix_index = tree->text_size - current_size;
    // printf(" [%d]\n", src->suffix_index);
  }
}

SuffixTree* newTree(char* text) {
  SuffixTree* tree = (SuffixTree*)malloc(sizeof(SuffixTree));
  tree->text = text;
  tree->text_size = strlen(tree->text);
  tree->root_end = (int*)malloc(sizeof(int));
  *(tree->root_end) = -1;
  tree->root = newNode(tree, -1, tree->root_end);

  // filling the atctive_point
  tree->active_point = (Point*)malloc(sizeof(Point));
  tree->active_point->active_node = tree->root;
  tree->active_point->active_length = 0;
  tree->active_point->active_edge = -1;

  tree->latest_node = NULL;
  tree->suffix_remaining = 0;
  tree->leaf_end = -1;
  tree->split_end = NULL;

  for (int i = 0; i < tree->text_size; i++) {
    addToTree(tree, i);
  }
  int current_size = 0;
  SetIndexOfSuffix(tree->root, current_size, tree);

  return tree;
}

int elementFound(char* text, char* pattern, int index, int start, int end) {
  for (int j = start; j <= end && pattern[index] != '\0'; j++, index++) {
    if (text[j] != pattern[index]) {
      return -1;
    }
  }
  if (pattern[index] == '\0') {
    return 1;
  }
  return 2;
}

void findAllMatchesDfs(Node* src) {
  if (result->size == 0) {
    result->array = (int*)malloc(sizeof(int) * (++(result->size)));
  }
  if (src->suffix_index > -1) {
    result->array[result->size - 1] = src->suffix_index;
    result->array =
        (int*)realloc(result->array, sizeof(int) * (++(result->size)));
    return;
  } else {
    for (int i = 0; i < MAX_SIZE; ++i) {
      if (src->children[i] != NULL) {
        findAllMatchesDfs(src->children[i]);
      }
    }
  }
}
int findSubstr(SuffixTree* tree, Node* src, char* pattern, int index) {
  // printf("эййй слыыыш\n");
  // printf("грань:");
  // for (int i = src->start; i <= *(src->end); i++) {
  //   printf("%c", tree->text[i]);
  // }
  // printf("\n");
  if (src == NULL) {
    return -1;
  }
  int on_edge = -1;

  if (src->start != -1) {
    on_edge = elementFound(tree->text, pattern, index, src->start, *(src->end));
    int edge_length = *(src->end) - src->start + 1;
    if (on_edge == -1) {
      return -1;
    }
    if (on_edge == 1) {  // мэтчится
      findAllMatchesDfs(src);
      return 1;
    }
    index += edge_length;
    if (src->children[(int)(pattern[index] - 'a')] != NULL) {
      return findSubstr(tree, src->children[(int)(pattern[index] - 'a')],
                        pattern, index);
    } else {
      // printf("тут -1 '%c' с индексом %d?\n", pattern[index], index);
      return -1;
    }

  } else {
    return findSubstr(tree, src->children[(int)(pattern[index] - 'a')], pattern,
                      index);
  }
}

void scanString(char** line) {
  char c = '\0';
  int size = 3;
  int max_size = 4;
  *line = (char*)malloc(size * sizeof(char));
  while ((c = getchar()) != '\n') {
    if (size == max_size) {
      max_size *= 2;
      *line = (char*)realloc(*line, max_size * sizeof(char));
    }
    *(*line + size - 3) = c;
    *(*line + size - 2) = '}';
    *(*line + size - 1) = '\0';
    size++;
  }
}

void destroyNodes(Node* src) {
  if (src == NULL) {
    return;
  }
  for (int i = 0; i < MAX_SIZE; i++) {
    if (src->children[i] != NULL) {
      destroyNodes(src->children[i]);
    }
  }
  if (src->suffix_index == -1) {
    free(src->end);
  }

  free(src);
}

void destroyTree(SuffixTree* tree) {
  destroyNodes(tree->root);
  free(tree->active_point);
  free(result);
  free(tree->text);
  free(tree);
  tree = NULL;
}

using namespace std;
int main(int arc, char* argv[]) {
  result->size = 0;
  char* text = NULL;
  scanString(&text);
  int length = strlen(text);
  SuffixTree* tree = newTree(text);
  int count = 0;
  char pattern[2048];
  // printf("строка:\"%s\"\n", text);
  while (cin >> pattern) {
    // printf("паттерн: \"%s\"\n", pattern);
    auto start_time = chrono::high_resolution_clock::now();
    int found = findSubstr(tree, tree->root, pattern, 0);
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
        end_time - start_time);
    std::cout << "Время выполнения: " << duration.count() << " миллисекунд"
              << std::endl;
    count++;
    if (result->size != 0) {
      printf("%d: ", count);
      BubbleSort(result->array, result->size - 1);
      for (int i = 0; i < result->size - 1; i++) {
        printf("%d", result->array[i] + 1);
        if (i < result->size - 2) {
          printf(", ");
        }
      }
      printf("\n");
      free(result->array);
    }
    result->size = 0;
  }

  destroyTree(tree);
  return 0;
}
