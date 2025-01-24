#include "vector.hh"
#include <concepts>
#include <print>
#include <random>

// least significant digit first radix sort
template <class T>
  requires std::integral<T>
void LSD(ns::vector<T> &A) {
  constexpr int BYTES = sizeof(T);
  constexpr int BITS_PER_BYTE = 8;
  constexpr int R = 1 << BITS_PER_BYTE;
  constexpr int MASK = R - 1;

  int n = A.size();
  ns::vector<T> aux(n);

  for (int d = 0; d < BYTES; d++) {
    ns::vector<int> count(R + 1, 0);
    for (int i = 0; i < n; i++) {
      int c = (A[i] >> BITS_PER_BYTE * d) & MASK;
      count[c + 1]++;
    }

    for (int r = 0; r < R; r++)
      count[r + 1] += count[r];

    if constexpr (std::is_signed_v<T>) {
      if (d == BYTES - 1) {
        int shift1 = count[R] - count[R / 2];
        int shift2 = count[R / 2];
        // shift right positive integer
        for (int r = 0; r < R / 2; r++)
          count[r] += shift1;
        // shift left negative integer
        for (int r = R / 2; r < R; r++)
          count[r] -= shift2;
      }
    }

    for (int i = 0; i < n; i++) {
      int c = (A[i] >> BITS_PER_BYTE * d) & MASK;
      aux[count[c]++] = A[i];
    }
    std::swap(A, aux);

    std::print("\n");
    std::ranges::for_each(A, [](auto x) { std::print("{}\t", x); });
    std::print("\n");
  }
}

int main() {
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution<int> rand(0x80000000, 0x7fffffff);

  ns::vector<int> A(8);
  for (auto &e : A) {
    e = rand(mt);
    std::print("{}\t", e);
  }
  std::print("\n");

  LSD(A);
  std::print("\n");
  std::ranges::for_each(A, [](auto x) { std::print("{}\t", x); });
  std::print("\n\n");
  std::print("{}", std::ranges::is_sorted(A) ? "Sorted" : "Unsorted");
  std::print("\n");
}