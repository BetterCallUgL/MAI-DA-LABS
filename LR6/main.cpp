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

// solution
int maxRectangleAreainHisto(int* hist, int size) {
  Stack* stack = NULL;
  stackInit(&stack);
  int max_area = 0, width = 0, height = 0;
  for (int i = 0; i <= size; i++) {
    while (stack->size != 0 &&
           (i == size || hist[stack->top->data] >= hist[i])) {
      height = hist[stack->top->data];
      pop(stack);
      width = (stack->size == 0) ? i : i - stack->top->data - 1;
      if (width * height > max_area) {
        max_area = width * height;
      }
    }
    push(stack, i);
  }

  destroyStack(&stack);
  return max_area;
}

int main() {
  int n, m;
  cin >> n >> m;
  int** matrix;
  allocateMatrix(n, m, &matrix);
  readMatrix(n, m, matrix);
  auto start_time = chrono::high_resolution_clock::now();
  int result = 0;
  int* array = (int*)malloc(sizeof(int) * m);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (matrix[i][j] == 0) {
        array[j]++;
      } else {
        array[j] = 0;
      }
    }
    int tmp = maxRectangleAreainHisto(array, m);
    result = (tmp > result) ? tmp : result;
  }
  auto end_time = chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);
  std::cout << "Время выполнения: " << duration.count() << " миллисекунд"
            << std::endl;
  cout << result;
  free(array);
  freeMatrix(n, &matrix);
  return 0;
}