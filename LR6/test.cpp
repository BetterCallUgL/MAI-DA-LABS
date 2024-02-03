#include <iostream>
using namespace std;
#include <chrono>

// stack realization
typedef struct Node {
  int data;
  Node* prev;
} Node;

typedef struct Stack {
  int size;
  Node* top;
} Stack;

void stackInit(Stack** src) {
  *src = (Stack*)malloc(sizeof(Stack));
  (*src)->size = 0;
  (*src)->top = NULL;
}

void push(Stack* src, int num) {
  Node* tmp = (Node*)malloc(sizeof(Node));
  tmp->data = num;
  tmp->prev = src->top;
  src->top = tmp;
  src->size++;
}

void pop(Stack* src) {
  Node* tmp = src->top;
  tmp = src->top;
  src->top = src->top->prev;
  free(tmp);
  src->size--;
}

void destroyStack(Stack** src) {
  while ((*src)->size != 0) {
    pop(*src);
  }
  free(*src);
  *src = NULL;
}
/*
end of stack realization
*/

// matrix realization
void allocateMatrix(int n, int m, int*** matrix) {
  *matrix = (int**)malloc(sizeof(int*) * n);
  for (int i = 0; i < n; i++) {
    *(*matrix + i) = (int*)malloc(sizeof(int) * m);
  }
}

void readMatrix(int n, int m, int** matrix) {
  char c = getchar();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      c = getchar();
      matrix[i][j] = (int)(c - '0');
    }
    c = getchar();
  }
}

void freeMatrix(int n, int*** matrix) {
  for (int i = 0; i < n; i++) {
    free(*(*matrix + i));
  }
  free(*matrix);
  *matrix = NULL;
}
/*
end of matrix realization
*/

int is_matrix(int i1, int j1, int i2, int j2, int** matrix) {
  int result = 1;
  for (int i = i1; i < i2; i++) {
    for (int j = j1; j < j2; j++) {
      if (matrix[i][j] == 1) {
        result = 0;
      }
    }
  }
  return result;
}
// solution

int main() {
  int n, m;
  cin >> n >> m;
  int** matrix;
  int result = 0;
  allocateMatrix(n, m, &matrix);
  readMatrix(n, m, matrix);
  auto start_time = chrono::high_resolution_clock::now();
  for (int i1 = 0; i1 < n - 1; i1++) {
    for (int j1 = 0; j1 < m - 1; j1++) {
      for (int i2 = i1 + 1; i2 < n; i2++) {
        for (int j2 = j1 + 1; j2 < m; j2++) {
          int tmp = (i2 - i1) * (j2 - j1);
          if (is_matrix(i1, j1, i2, j2, matrix) && tmp > result) {
            result = tmp;
          }
        }
      }
    }
  }
  cout << "ХУЙЙ";
  auto end_time = chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);
  std::cout << "Время выполнения: " << duration.count() << " миллисекунд"
            << std::endl;
  cout << result << "\n";
  freeMatrix(n, &matrix);
  return 0;
}