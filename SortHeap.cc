#include "vector.hh"
#include <concepts>
#include <print>
#include <random>

/**
 *   (k-1)/2
 *      |
 *      k
 *    /   \
 *  2k+1  2k+2
 */

template <typename compar>
  requires std::totally_ordered<compar>
struct heap {
  static void sort(ns::vector<compar> &pq) {
    int n{pq.size() - 1};
    // heapify phase
    for (int k = (n - 1) / 2; k >= 0; --k)
      sink(pq, k, n);
    // sortdown phase
    while (n > 0) {
      std::swap(pq[0], pq[n--]);
      sink(pq, 0, n);
    }
  }
  static void sink(ns::vector<compar> &pq, int k, int n) {
    while (2 * k + 1 <= n) {
      int j{2 * k + 1};
      if (j < n && pq[j] < pq[j + 1])
        ++j;
      if (pq[k] >= pq[j])
        break;
      std::swap(pq[k], pq[j]);
      k = j;
    }
  }
};

int main() {
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution rand(10, 99);

  ns::vector<int> a(16);
  for (auto &e : a) {
    e = rand(mt);
    std::print("{}\t", e);
  }
  std::print("\n\n");

  heap<int>::sort(a);
  std::ranges::for_each(a, [](auto x) { std::print("{}\t", x); });
  std::print("\n\n");
  std::print("{}", std::ranges::is_sorted(a) ? "Sorted" : "Unsorted");
  std::print("\n");
}