#include "vector.hh"
#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstdlib>
#include <ctime>
#include <print>

// 折半查找
template <typename T>
  requires std::totally_ordered<T>
int bisectionSearch(const T &key, const ns::vector<T> &A) {
  assert(std::ranges::is_sorted(A));
  int lo(0), hi(A.size() - 1);
  while (lo <= hi) {
    int mid{lo + (hi - lo) / 2};
    if (key < A[mid])
      hi = mid - 1;
    else if (key > A[mid])
      lo = mid + 1;
    // key == A[mid]
    else
      return mid;
  }
  return -1;
}

// 斐波那契查找
template <class T>
  requires std::totally_ordered<T>
int fibonacciSearch(const T &key, const ns::vector<T> &A) {
  assert(std::ranges::is_sorted(A));
  int lo = 0, hi = A.size() - 1;
  int alpha{0}, beta{1}, x;
  do {
    x = alpha + beta;
    alpha = beta;
    beta = x;
  } while (beta < hi - lo);

  while (lo <= hi) {
    while (beta > hi - lo) {
      x = beta - alpha;
      beta = alpha;
      alpha = x;
    }
    int pivot{lo + beta};
    if (key < A[pivot])
      hi = pivot - 1;
    else if (key > A[pivot])
      lo = pivot + 1;
    else
      return pivot;
  }
  return -1;
}

// 大于等于查找目标的最小元素
template <typename T>
  requires std::totally_ordered<T>
int rightBisection(const T &key, const ns::vector<T> &A) {
  assert(std::ranges::is_sorted(A));
  int lo(0), hi(A.size() - 1);
  while (lo < hi) {
    int mid{(lo + hi) >> 1};
    if (key <= A[mid])
      hi = mid;
    // key > A[mid]
    else
      lo = mid + 1;
  }
  assert(lo == hi);
  return hi;
}

// 小于等于查找目标的最大元素
template <typename T>
  requires std::totally_ordered<T>
int leftBisection(const T &key, const ns::vector<T> &A) {
  assert(std::ranges::is_sorted(A));
  int lo(0), hi(A.size() - 1);
  while (lo < hi) {
    int mid = (lo + hi + 1) >> 1;
    if (key >= A[mid])
      lo = mid;
    // key < A[mid]
    else
      hi = mid - 1;
  }
  assert(lo == hi);
  return lo;
}

int main() {
  std::srand(std::time(nullptr));
  ns::vector<int> vec{1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29};
  int num = std::rand() % vec.back();
  std::print("number\t{}\n", num);
  for (const auto &e : vec)
    std::print("{}\t", e);
  std::print("\n");

  int r{bisectionSearch<int>(num, vec)};
  if (r != -1) {
    for (int i = 0; i < r; ++i)
      std::print("\t");
    std::print("{}", vec[r]);
  } else
    std::print("{}", r);
  std::print("\n");
  assert(bisectionSearch<int>(vec.front() - 1, vec) == -1);
  assert(bisectionSearch<int>(vec.back() + 1, vec) == -1);

  int s{fibonacciSearch<int>(num, vec)};
  if (s != -1) {
    for (int i = 0; i < s; ++i)
      std::print("\t");
    std::print("{}", vec[s]);
  } else
    std::print("{}", s);
  std::print("\n");
  assert(fibonacciSearch<int>(vec.front() - 1, vec) == -1);
  assert(fibonacciSearch<int>(vec.back() + 1, vec) == -1);

  assert(r == s);

  int t{rightBisection<int>(num, vec)};
  for (int i = 0; i < t; ++i)
    std::print("\t");
  std::print("{}", vec[t]);
  std::print("\n");

  int u{leftBisection<int>(num, vec)};
  for (int i = 0; i < u; ++i)
    std::print("\t");
  std::print("{}", vec[u]);
  std::print("\n");

  assert(t == u || t == u + 1);
}