#include "vector.hh"
#include <algorithm>
#include <functional>
#include <print>
#include <random>

// 冒泡排序
template <typename compar>
  requires std::totally_ordered<compar>
void bubble(ns::vector<compar> &A) {
  int n = A.size();
  bool sorted{false};
  while (!sorted) {
    sorted = true;
    for (int i = 1; i < n; ++i)
      if (A[i] < A[i - 1]) {
        std::swap(A[i - 1], A[i]);
        sorted = false;
      }
    --n;
  }
}

// 单选择排序
template <typename compar>
  requires std::totally_ordered<compar>
void selection(ns::vector<compar> &A) {
  for (int i(0), sz(A.size()); i < sz; ++i) {
    int min{i};
    for (int j = i + 1; j < sz; ++j)
      if (A[j] < A[min])
        min = j;
    std::swap(A[i], A[min]);
  }
}

// 线性插入排序
template <typename compar>
  requires std::totally_ordered<compar>
void insertion(ns::vector<compar> &A) {
  for (int i(1), sz(A.size()); i < sz; ++i) {
    compar current{A[i]};
    int j{i};
    for (; j >= 1 && current < A[j - 1]; --j)
      A[j] = A[j - 1];
    A[j] = current;
  }
}

// 折半插入排序
template <typename compar>
  requires std::totally_ordered<compar>
void leftBisectionInsertion(ns::vector<compar> &A) {
  for (int i(1), sz(A.size()); i < sz; ++i) {
    compar var{A[i]};
    int lo{0}, hi{i - 1};

    while (lo <= hi) {
      int mid{lo + (hi - lo) / 2};
      if (var < A[mid])
        hi = mid - 1;
      // var >= A[mid]
      else
        lo = mid + 1;
    }
    for (int j = i; j > lo; --j)
      A[j] = A[j - 1];

    A[lo] = var;
  }
}

template <typename compar>
  requires std::totally_ordered<compar>
void rightBisectionInsertion(ns::vector<compar> &A) {
  for (int sz(A.size()), i(sz - 1); i >= 0; --i) {
    compar var{A[i]};
    int lo{i + 1}, hi{sz - 1};

    while (lo <= hi) {
      int mid{lo + (hi - lo) / 2};
      if (var <= A[mid])
        hi = mid - 1;
      // var > A[mid]
      else
        lo = mid + 1;
    }
    for (int j = i; j < hi; ++j)
      A[j] = A[j + 1];

    A[hi] = var;
  }
}

// 希尔排序
template <typename compar>
  requires std::totally_ordered<compar>
void shell(ns::vector<compar> &A) {
  int sz(A.size()), h{1};
  // 3 * h + 1 <= sz
  while (h < sz / 3)
    h = 3 * h + 1;
  while (h >= 1) {
    for (int i = h; i < sz; ++i) {
      compar var{A[i]};
      int j{i};
      while (j >= h && A[j - h] > var) {
        A[j] = A[j - h];
        j = j - h;
      }
      A[j] = var;
    }
    h = h / 3;
  }
}

template <class T>
void printSort(std::function<void(ns::vector<T> &)> F, ns::vector<T> A) {
  F(A);
  std::ranges::for_each(A, [](auto x) { std::print("{}\t", x); });
  std::print("\n");
  std::print("{}", std::ranges::is_sorted(A) ? "Sorted" : "Unsorted");
  std::print("\n\n");
}

int main() {
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution rand(10, 99);
  ns::vector<int> A(16);
  for (auto &e : A) {
    e = rand(mt);
    std::print("{}\t", e);
  }
  std::print("\n\n");

  std::print("bubble\n");
  printSort<int>(bubble<int>, A);

  std::print("selection\n");
  printSort<int>(selection<int>, A);

  std::print("insertion\n");
  printSort<int>(insertion<int>, A);

  std::print("leftBisectionInsertion\n");
  printSort<int>(leftBisectionInsertion<int>, A);

  std::print("rightBisectionInsertion\n");
  printSort<int>(rightBisectionInsertion<int>, A);

  std::print("shell\n");
  printSort<int>(shell<int>, A);
}
