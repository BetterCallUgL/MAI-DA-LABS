#include <algorithm>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>
#include <chrono>
using namespace std;

// Структура для представления ребра графа
struct Edge {
  int to, weight;
  Edge(int t, int w) : to(t), weight(w) {}
};

// Рекурсивная функция для поиска кратчайшего пути
void dfs(const vector<vector<Edge>>& graph, int current, int finish,
         int& minDistance, int currentDistance, vector<bool>& visited) {
  if (current == finish) {
    minDistance = min(minDistance, currentDistance);
    return;
  }

  visited[current] = true;

  for (const Edge& edge : graph[current]) {
    if (!visited[edge.to]) {
      dfs(graph, edge.to, finish, minDistance, currentDistance + edge.weight,
          visited);
    }
  }

  visited[current] =
      false;  // Отмечаем вершину как непосещенную при возврате из рекурсии
}

// Функция для нахождения кратчайшего пути с использованием DFS
int shortestPathDFS(const vector<vector<Edge>>& graph, int start, int finish) {
  int n = graph.size();
  int minDistance = INT32_MAX;
  vector<bool> visited(n, false);

  dfs(graph, start, finish, minDistance, 0, visited);

  return minDistance;
}

int main() {
  int n, m;
  cin >> n >> m;  // Вводим количество вершин и рёбер

  vector<vector<Edge>> graph(n);
  int startVertex, finishVertex;
  cin >> startVertex >> finishVertex;  // Вводим начальную и конечную вершины

  for (int i = 0; i < m; ++i) {
    int from, to, weight;
    cin >> from >> to >> weight;  // Вводим рёбра и их веса
    graph[from - 1].emplace_back(to - 1, weight);
    graph[to - 1].emplace_back(from - 1, weight);
  }
    auto start_time = chrono::high_resolution_clock::now();
  int result = shortestPathDFS(graph, startVertex - 1, finishVertex - 1);
  auto end_time = chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);
  std::cout << "Время выполнения: " << duration.count() << " миллисекунд"
            << std::endl;
  // Вывод результата
  // if (result == INT32_MAX) {
  //   cout << "Путь из " << startVertex << " в " << finishVertex << " не найден."
  //        << endl;
  // } else {
  //   cout << "Кратчайшая длина пути из " << startVertex << " в " << finishVertex
  //        << ": " << result << endl;
  // }

  return 0;
}