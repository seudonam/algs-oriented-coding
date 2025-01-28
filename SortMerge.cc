#include "vector.hh"
#include <algorithm>
#include <concepts>
#include <print>
#include <random>

template <typename compar>
  requires std::totally_ordered<compar>
struct MergeTD {
  static void sort(ns::vector<compar> &a) {
    ns::vector<compar> aux(a.size());
    sort(a, aux, 0, a.size() - 1);
  }
  static void sort(ns::vector<compar> &a, ns::vector<compar> &aux, int lo,
                   int hi) {
    if (lo >= hi)
      return;
    int mid{lo + (hi - lo) / 2};
    sort(a, aux, lo, mid);
    sort(a, aux, mid + 1, hi);
    merge(a, aux, lo, mid, hi);
  }
  static void merge(ns::vector<compar> &a, ns::vector<compar> &aux, int lo,
                    int mid, int hi) {
    for (int k = lo; k <= hi; ++k)
      aux[k] = a[k];

    int i{lo}, j{mid + 1};
    for (int k = lo; k <= hi; ++k)
      if (i > mid)
        a[k] = aux[j++];
      else if (j > hi)
        a[k] = aux[i++];
      else if (aux[j] < aux[i])
        a[k] = aux[j++];
      // aux[j] >= aux[i]
      else
        a[k] = aux[i++];
  }
};

template <typename compar>
  requires std::totally_ordered<compar>
struct MergeBU {
  static void sort(ns::vector<compar> &a) {
    int n = a.size();
    ns::vector<compar> aux(n);
    for (int len = 1; len < n; len = len + len) {
      for (int lo = 0; lo < n - len; lo += len + len) {
        int mid{lo + len - 1};
        int hi{std::min(lo + len + len - 1, n - 1)};

        for (int k = lo; k <= hi; ++k)
          aux[k] = a[k];

        int i{lo}, j{mid + 1};
        for (int k = lo; k <= hi; ++k) {
          if (i > mid)
            a[k] = aux[j++];
          else if (j > hi)
            a[k] = aux[i++];
          else if (aux[j] < aux[i])
            a[k] = aux[j++];
          // aux[j] >= aux[i]
          else
            a[k] = aux[i++];
        }
      }
    }
  }
};

template <typename compar>
  requires std::totally_ordered<compar>
struct Merge408 {
  static void sort(ns::vector<compar> &a) {
    ns::vector<compar> aux(a.size());
    sort(a, aux, 0, a.size() - 1);
  }
  static void sort(ns::vector<compar> &a, ns::vector<compar> &aux, int low,
                   int high) {
    if (high <= low)
      return;
    int mid{low + (high - low) / 2};
    sort(a, aux, low, mid);
    sort(a, aux, mid + 1, high);
    merge(a, aux, low, mid, high);
  }
  static void merge(ns::vector<compar> &a, ns::vector<compar> &aux, int low,
                    int mid, int high) {
    for (int k = low; k <= high; ++k)
      aux[k] = a[k];
    int i{low}, j{mid + 1}, k{low};
    while (i <= mid && j <= high) {
      if (aux[i] <= aux[j])
        a[k] = aux[i++];
      else
        a[k] = aux[j++];
      ++k;
    }
    while (i <= mid)
      a[k++] = aux[i++];
    /*
    while (j <= high)
      a[k++] = aux[j++];
    */
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

  std::print("MergeTD\n");
  printSort<MergeTD<int>, int>(A);
  printSort<MergeTD<int>, int>({4, 4, 4, 4});

  std::print("MergeBU\n");
  printSort<MergeBU<int>, int>(A);
  printSort<MergeBU<int>, int>({4, 4, 4, 4});

  std::print("Merge408\n");
  printSort<Merge408<int>, int>(A);
  printSort<Merge408<int>, int>({4, 4, 4, 4});
}