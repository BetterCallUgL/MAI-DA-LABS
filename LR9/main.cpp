#include <iostream>
#include <limits>
#include <set>
#include <vector>
#include <chrono>
using namespace std;

struct weight_edge {
  int where;
  unsigned long long distance;
};

struct vertex {
  unsigned long long distance;
  int id;
};

bool operator<(const vertex &a, const vertex &b) {
  return a.distance < b.distance;
}

void dijkstra_solution(vector<vector<weight_edge>> graph, int start,
                       int finish) {
  set<vertex> priority_queue;
  int size = graph.size();
  vector<bool> visited(size, false);
  vector<unsigned long long> distances(size, UINT64_MAX);
  distances[start] = 0;
  priority_queue.insert({distances[start], start});

  while (!priority_queue.empty()) {
    vertex cur_vertex = *priority_queue.begin();
    priority_queue.erase(priority_queue.begin());
    int vertex1 = cur_vertex.id;

    if (!visited[vertex1]) {
      for (weight_edge wedge : graph[vertex1]) {
        int vertex2 = wedge.where;
        int length = wedge.distance;
        if (distances[vertex1] + length < distances[vertex2]) {
          distances[vertex2] = distances[vertex1] + length;
          priority_queue.insert({distances[vertex2], vertex2});
        }
      }
      visited[vertex1] = true;
      if (vertex1 == finish) {
        break;
      }
    }
  }

  // if (distances[finish] == UINT64_MAX) {
  //   cout << "No solution";
  // } else {
  //   cout << distances[finish];
  // }
}

int main() {
  int n, m, start, finish;
  cin >> n >> m >> start >> finish;
  --start;
  --finish;
  vector<vector<weight_edge>> graph(n);

  for (int i = 0; i < m; ++i) {
    int vertex1, vertex2;
    unsigned long long distance;
    cin >> vertex1 >> vertex2 >> distance;
    --vertex1;
    --vertex2;
    graph[vertex1].push_back({vertex2, distance});
    graph[vertex2].push_back({vertex1, distance});
  }
  auto start_time = chrono::high_resolution_clock::now();
  dijkstra_solution(graph, start, finish);
    auto end_time = chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);
  std::cout << "Время выполнения: " << duration.count() << " миллисекунд"
            << std::endl;
}