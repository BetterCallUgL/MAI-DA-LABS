#include <algorithm>
#include <chrono>
#include <climits>
#include <iostream>
#include <stack>
#include <vector>
#include
using namespace std;

typedef struct trio {
  int first;
  int second;
  int id;
} trio;

void solution(vector<trio> sections, int n, int m) {
  vector<trio> answer;
  int k = 0;
  int index = sections.size() - 1;
  int lower_bound = 0;
  bool flag = 0;
  while (index >= 0) {
    int cur_index = -1;
    for (int j = index; j >= 0; j--) {
      if (sections[j].first <= lower_bound) {
        cur_index = j;
      }
    }

    if (cur_index == -1) {
      flag = 1;
      break;
    }

    answer.push_back(sections[cur_index]);
    k += 1;
    lower_bound = sections[cur_index].second;
    while (index >= cur_index) {
      index--;
      sections.pop_back();
    }

    if (lower_bound >= m) {
      break;
    }
  }

  sort(answer.begin(), answer.end(),
       [](trio a, trio b) { return a.id < b.id; });

  if (flag || lower_bound < m) {
    cout << 0;
  } else {
    cout << k << '\n';
    for (int i = 0; i < k; i++) {
      cout << answer[i].first << ' ' << answer[i].second << '\n';
    }
  }
}

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
  // solution
  solution(sections, n, m);
  auto end_time = chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);
  std::cout << "Время выполнения: " << duration.count() << " миллисекунд"
            << std::endl;

  return 0;
}