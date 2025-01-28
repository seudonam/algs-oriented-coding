#pragma once
#include "deque.hh"
#include <cassert>
#include <concepts>

/**
 *     k/2
 *      |
 *      k
 *    /   \
 *   2k  2k+1
 */

template <typename T>
  requires std::totally_ordered<T>
struct MinPQ {
  T *pq;
  int maxN;
  int n;

  MinPQ(int maxN) : pq{new T[maxN + 1]}, maxN{maxN}, n{0} {}

  MinPQ(const ns::deque<T> &deck)
      : pq{new T[deck.size() + 1]}, maxN(deck.size()), n(deck.size()) {
    for (int i = 0; i < n; i++)
      pq[i + 1] = deck[i];
    // bottom up sink
    for (int k = n / 2; k >= 1; k--)
      sink(k);
    assert(isMinHeap(1));
  }

  ~MinPQ() { delete[] pq; }

  bool empty() { return n == 0; }

  void insert(T e) {
    assert(n < maxN);
    n++;
    pq[n] = e;
    swim(n);
    assert(isMinHeap(1));
  }

  T delMin() {
    assert(n > 0);
    T min{pq[1]};
    std::swap(pq[1], pq[n]);
    n--;
    sink(1);
    assert(isMinHeap(1));
    return min;
  }

  void swim(int k) {
    while (k > 1 && pq[k / 2] > pq[k]) {
      std::swap(pq[k / 2], pq[k]);
      k = k / 2;
    }
  }

  void sink(int k) {
    while (2 * k <= n) {
      int j{2 * k};
      if (j < n && pq[j] > pq[j + 1])
        j++;
      if (pq[k] <= pq[j])
        break;
      std::swap(pq[k], pq[j]);
      k = j;
    }
  }

  bool isMinHeap(int k) {
    if (k > n)
      return true;
    int left{2 * k}, right{2 * k + 1};
    if (left <= n && pq[k] > pq[left])
      return false;
    if (right <= n && pq[k] > pq[right])
      return false;
    return isMinHeap(left) && isMinHeap(right);
  }
};

template <typename T>
  requires std::totally_ordered<T>
struct IndexMinPQ {
  int *pq;
  int *qp;
  T *keys;
  int maxN;
  int n;

  IndexMinPQ(int maxN)
      : pq{new int[maxN + 1]}, qp{new int[maxN + 1]}, keys{new T[maxN + 1]},
        maxN{maxN}, n{0} {
    for (int i = 0; i <= maxN; i++)
      qp[i] = -1;
  }

  ~IndexMinPQ() {
    delete[] pq;
    delete[] qp;
    delete[] keys;
  }

  bool empty() const { return n == 0; }
  bool contains(int i) const { return qp[i] != -1; }

  void insert(int i, T key) {
    assert(n < maxN);
    n++;
    pq[n] = i;
    qp[i] = n;
    keys[i] = key;
    swim(n);
    assert(isMinHeap(1));
  }

  int delMin() {
    assert(n > 0);
    int min{pq[1]};
    exch(1, n);
    n--;
    sink(1);
    qp[min] = -1;
    assert(isMinHeap(1));
    return min;
  }

  void decreaseKey(int i, T key) {
    keys[i] = key;
    swim(qp[i]);
  }

  void increaseKey(int i, T key) {
    keys[i] = key;
    sink(qp[i]);
  }

  void swim(int k) {
    while (k > 1 && greater(k / 2, k)) {
      exch(k / 2, k);
      k = k / 2;
    }
  }

  void sink(int k) {
    while (2 * k <= n) {
      int j{2 * k};
      if (j < n && greater(j, j + 1))
        j++;
      if (!greater(k, j))
        break;
      exch(k, j);
      k = j;
    }
  }

  bool greater(int i, int j) { return keys[pq[i]] > keys[pq[j]]; }

  void exch(int i, int j) {
    int x{std::move(pq[i])};
    pq[i] = std::move(pq[j]);
    pq[j] = std::move(x);
    qp[pq[i]] = i;
    qp[pq[j]] = j;
  }

  bool isMinHeap(int k) {
    if (k > n)
      return true;
    int left{2 * k}, right{2 * k + 1};
    if (left <= n && greater(k, left))
      return false;
    if (right <= n && greater(k, right))
      return false;
    return isMinHeap(left) && isMinHeap(right);
  }
};

template <class T, class Comparator> struct PQ {
  T *pq;
  int n;
  int maxN;
  Comparator comparator;

  PQ(int maxN) : pq{new T[maxN + 1]}, n{0}, maxN{maxN} {}

  PQ(int maxN, auto comparator)
      : pq{new T[maxN + 1]}, n{0}, maxN{maxN}, comparator{comparator} {}

  ~PQ() { delete[] pq; }

  bool empty() { return n == 0; }

  void push(T e) {
    assert(n < maxN);
    n++;
    pq[n] = e;
    swim(n);
    assert(isHeap(1));
  }

  T pop() {
    assert(n > 0);
    T top{pq[1]};
    std::swap(pq[1], pq[n]);
    n--;
    sink(1);
    assert(isHeap(1));
    return top;
  }

  void swim(int k) {
    while (k > 1 && comparator(pq[k / 2], pq[k])) {
      std::swap(pq[k / 2], pq[k]);
      k = k / 2;
    }
  }

  void sink(int k) {
    while (2 * k <= n) {
      int j{2 * k};
      if (j < n && comparator(pq[j], pq[j + 1]))
        j++;
      if (!comparator(pq[k], pq[j]))
        break;
      std::swap(pq[k], pq[j]);
      k = j;
    }
  }

  bool isHeap() { return isHeap(1); }
  bool isHeap(int k) {
    if (k > n)
      return true;
    int left{2 * k}, right{2 * k + 1};
    if (left <= n && comparator(pq[k], pq[left]))
      return false;
    if (right <= n && comparator(pq[k], pq[right]))
      return false;
    return isHeap(left) && isHeap(right);
  }
};

template <class T> struct Smaller {
  bool operator()(T a, T b) { return a < b; }
};

template <class T> struct Bigger {
  bool operator()(T a, T b) { return a > b; }
};