#include <math.h>

#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

typedef struct trio {
  int first;
  int second;
  int id;
} trio;

int main() {
  int n, m;
  cin >> n;
  vector<trio> sections;

  // input
  for (int i = 0; i < n; i++) {
    int a, b;
    cin >> a >> b;
    if (b >= 0) {
      sections.push_back(trio{a, b, i});
    }
  }
  cin >> m;

  // sort
  auto start_time = chrono::high_resolution_clock::now();

  sort(sections.begin(), sections.end(),
       [](trio a, trio b) { return a.second > b.second; });

  int result = 0;
  for (int i = 0; i < pow(2, n); i++) {
    int cur_size = 0;
    vector<trio> cur_sections;
    for (int j = 0; j < sections.size(); j++) {
      if ((i >> j) & 1 == 1) {
        cur_sections.push_back(sections[j]);
      }
    }
    int k = 0;
    for (int j = 0; j < cur_sections.size() - 1; j++) {
        k++;
    }
  }
  auto end_time = chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);
  std::cout << "Время выполнения: " << duration.count() << " миллисекунд"
            << std::endl;
}