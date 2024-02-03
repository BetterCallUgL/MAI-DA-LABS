#include <chrono>
#include <cstring>
#include <ctime>
#include <iostream>
#include <chrono>

using namespace std;
void naiveSearch(const char* haystack, const char* needle) {
  int haystackLen = strlen(haystack);
  int needleLen = strlen(needle);

  for (int i = 0; i <= haystackLen - needleLen; ++i) {
    int j;
    for (j = 0; j < needleLen; ++j) {
      if (haystack[i + j] != needle[j]) {
        break;  // несовпадение символов, переходим к следующему начальному
                // индексу
      }
    }
    if (j == needleLen) {
      std::cout << "Подстрока найдена в позиции: " << i << std::endl;
    }
  }
}

int main() {
  const int maxLength = 100000;  // Максимальная длина ввода
  char haystack[maxLength];
  char needle[maxLength];

  std::cout << "Введите строку (где искать): ";
  std::cin.getline(haystack, maxLength);

  std::cout << "Введите подстроку (что искать): ";
  std::cin.getline(needle, maxLength);
    auto start_time = chrono::high_resolution_clock::now();
    naiveSearch(haystack,needle);
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
        end_time - start_time);
            std::cout << "Время выполнения: " << duration.count() << " миллисекунд"
              << std::endl;
  return 0;
}