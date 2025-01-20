#include "vector.hh"
#include <cassert>
#include <concepts>
#include <ctime>
#include <print>

// 线性查找
template <typename compar>
  requires std::equality_comparable<compar>
int linearSearch(const compar &key, const ns::vector<compar> &A) {
  for (int i(0), sz(A.size()); i < sz; ++i)
    if (A[i] == key)
      return i;
  return -1;
}

// 分块查找
template <typename compar>
  requires std::totally_ordered<compar>
int blockSearch(const compar &key, const ns::vector<compar> &A) {
  assert(std::ranges::is_sorted(A));
  int n = A.size(), block = 1;
  while (block * block <= n)
    block++;

  int hi = 0;
  for (; hi < n; hi += block) {
    if (key < A[hi])
      break;
  }

  if (hi > 0) {
    int i{hi - block};
    if (n <= hi)
      hi = n;
    for (; i < hi; i++)
      if (key == A[i])
        return i;
  }

  return -1;
}

int main() {
  std::srand(std::time(nullptr));
  ns::vector<int> vec{1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29};

  int num{std::rand() % vec.back()};
  std::print("target\t{}\n", num);
  std::print("vector\t");
  for (const auto &e : vec)
    std::print("{}\t", e);
  std::print("\n");

  int r{linearSearch<int>(num, vec)};
  std::print("linear\t");
  if (r != -1) {
    for (int i = 0; i < r; ++i)
      std::print("\t");
    std::print("{}", vec[r]);
  } else
    std::print("{}", r);
  std::print("\n");
  assert(linearSearch<int>(vec.front() - 1, vec) == -1);
  assert(linearSearch<int>(vec.back() + 1, vec) == -1);

  int s{blockSearch<int>(num, vec)};
  std::print("block\t");
  if (s != -1) {
    for (int i = 0; i < s; ++i)
      std::print("\t");
    std::print("{}", vec[s]);
  } else
    std::print("{}", s);
  std::print("\n");
  assert(blockSearch<int>(vec.front() - 1, vec) == -1);
  assert(blockSearch<int>(vec.back() + 1, vec) == -1);

  assert(r == s);
}