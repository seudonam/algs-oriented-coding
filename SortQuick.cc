#include "vector.hh"
#include <algorithm>
#include <cassert>
#include <concepts>
#include <print>
#include <random>

template <typename compar>
  requires std::totally_ordered<compar>
struct Quick226 {
  static void sort(ns::vector<compar> &A) {
    std::ranges::shuffle(A, std::mt19937(std::random_device{}()));
    sort(A, 0, A.size() - 1);
  }
  static void sort(ns::vector<compar> &A, int lo, int hi) {
    if (lo >= hi)
      return;
    int p{partition113(A, lo, hi)};
    sort(A, lo, p - 1);
    sort(A, p + 1, hi);
  }

  static int partition226(ns::vector<compar> &A, int lo, int hi) {
    int i{lo}, j{hi + 1};
    while (true) {
      while (A[--j] > A[lo])
        if (j == lo)
          break;
      while (A[++i] < A[lo])
        if (i == hi)
          break;
      if (i >= j)
        break;
      std::swap(A[i], A[j]);
    }
    assert(i == j || i - j == 1);
    std::swap(A[lo], A[j]);
    return j;
  }

  static int partition113(ns::vector<compar> &A, int lo, int hi) {
    int i{lo}, j{hi + 1};
    while (true) {
      do
        i++;
      while (A[i] < A[lo] && i < hi);
      do
        j--;
      while (A[j] > A[lo] && j > lo);
      if (i >= j)
        break;
      std::swap(A[i], A[j]);
    }
    assert(i == j || i - j == 1);
    std::swap(A[lo], A[j]);
    return j;
  }
};

template <typename compar>
  requires std::totally_ordered<compar>
struct Quick408 {
  static void sort(ns::vector<compar> &A) {
    std::ranges::shuffle(A, std::mt19937(std::random_device{}()));
    sort(A, 0, A.size() - 1);
  }
  static void sort(ns::vector<compar> &A, int lo, int hi) {
    if (lo >= hi)
      return;
    int p{partition912(A, lo, hi)};
    sort(A, lo, p - 1);
    sort(A, p + 1, hi);
  }

  static int partition408(ns::vector<compar> &A, int lo, int hi) {
    compar pivot{A[lo]};
    do {
      while (lo < hi && A[hi] >= pivot)
        --hi;
      A[lo] = A[hi];
      while (lo < hi && A[lo] <= pivot)
        ++lo;
      A[hi] = A[lo];
    } while (lo < hi);
    assert(lo == hi);
    A[lo] = pivot;
    return lo;
  }

  static int partition660(ns::vector<compar> &A, int lo, int hi) {
    compar pivot{A[lo]};
    do {
      while (lo < hi && A[hi] > pivot)
        --hi;
      if (lo < hi)
        A[lo++] = A[hi];
      while (lo < hi && A[lo] < pivot)
        ++lo;
      if (lo < hi)
        A[hi--] = A[lo];
    } while (lo < hi);
    assert(lo == hi);
    A[lo] = pivot;
    return lo;
  }

  static int partition912(ns::vector<compar> &A, int lo, int hi) {
    compar pivot{A[lo]};
    do {
      while (lo < hi)
        if (A[hi] > pivot)
          --hi;
        else {
          A[lo++] = A[hi];
          break;
        }
      while (lo < hi)
        if (A[lo] < pivot)
          ++lo;
        else {
          A[hi--] = A[lo];
          break;
        }
    } while (lo < hi);
    assert(lo == hi);
    A[lo] = pivot;
    return lo;
  }
};

template <typename compar>
  requires std::totally_ordered<compar>
struct Quick3way {
  static void sort(ns::vector<compar> &A) {
    std::ranges::shuffle(A, std::mt19937(std::random_device{}()));
    sort(A, 0, A.size() - 1);
  }
  static void sort(ns::vector<compar> &A, int lo, int hi) {
    if (lo >= hi)
      return;
    int lt{lo}, gt{hi};
    compar pivot{A[lo]};
    int i{lo + 1};
    while (i <= gt) {
      if (A[i] < pivot)
        std::swap(A[lt++], A[i++]);
      else if (A[i] > pivot)
        std::swap(A[i], A[gt--]);
      else if (A[i] == pivot)
        ++i;
    }
    sort(A, lo, lt - 1);
    sort(A, gt + 1, hi);
  }
};

template <class S, class T> void printSort(ns::vector<T> A) {
  S::sort(A);
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

  std::print("Quick226\n");
  printSort<Quick226<int>, int>(A);
  printSort<Quick226<int>, int>({4, 4, 4, 4});

  std::print("Quick408\n");
  printSort<Quick408<int>, int>(A);
  printSort<Quick408<int>, int>({4, 4, 4, 4});
}