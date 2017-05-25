#include <cstdio>
#include <cstdlib>

#include "lru.hpp"

int main() {
  Lru<int, int> lru(5);
  for (int i = 1; i <= 10; ++i) {
    lru.Push(i, i);
  }

  auto print = [](int& value) { printf("%d,", value); };
  printf("origin: cap %lu, size %lu: ", lru.GetCapacity(), lru.GetSize());
  lru.ForEach(print);
  for (int i = 0; i < 20; ++i) {
    int key = std::rand() % 10 + 1;
    int result = 0;
    bool get = false;
    std::tie(result, get) = lru.Find(key);
    if (get) {
      printf("\nFind: %d, result %d,  : ", key, result);
    } else {
      printf("\nFind: %d, result not found.  ", key);
    }
    lru.ForEach(print);
  }

  lru.Clear();
  printf("\norigin: cap %lu, size %lu", lru.GetCapacity(), lru.GetSize());
  lru.ForEach(print);
  return 0;
}
