#include "Cycle.hh"
#include "Graph.hh"
#include "deque.hh"
#include "vector.hh"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <print>
#include <type_traits>

struct str {
  char buffer[512];
  char *pointer;
  str() : pointer(buffer) {}
};

auto main() -> int {
  double infi{-std::numeric_limits<double>::infinity()};
  double min{std::numeric_limits<double>::min()};
  std::print("{}\t{}\t", infi < min, infi + 8 == infi - 4);

  int a[2]{};
  for (int i = 0; i < 2; ++i)
    std::print("{}\t", a[i]);
  std::print("{}\t\n", __cplusplus);
  std::print("string is_trivially_copyable {}\n",
             std::is_trivially_copyable_v<std::string>);
  std::print("str is_trivially_copyable {}\n",
             std::is_trivially_copyable_v<str>);
  std::print("Edge is_trivially_copyable {}\n",
             std::is_trivially_copyable_v<Edge>);
  std::print("Graph is_trivially_copyable {}\n",
             std::is_trivially_copyable_v<Graph>);
  constexpr int num{9};

  // vector
  ns::vector<int> vec(2);
  std::fill(vec.begin(), vec.end(), 4);
  vec.push_back(0);
  assert(vec.back() == 0);
  vec.push_back(1);
  assert(vec.back() == 1);
  vec.pop_back();
  vec.pop_back();
  assert(vec.capacity() == 4);
  assert(vec.size() == 2);
  for (int i = 0; i < num; ++i) {
    int before = vec.capacity();
    int n = vec.size();
    ns::vector<int> compar(vec);
    for (int i = 0; i < n; i++)
      assert(compar[i] == vec[i]);
    vec.push_back(std::rand() % 90 + 10);
    int after = vec.capacity();
    if (before != after) {
      for (int i = 0; i < n; i++)
        assert(compar[i] == vec[i]);
    }
  }
  std::print("\nVECTOR\t");
  std::for_each(vec.begin(), vec.end(), [](auto x) { std::print("{}\t", x); });

  for (int i = 0; i < num; ++i) {
    int before = vec.capacity();
    int n = vec.size();
    ns::vector<int> compar = vec;
    for (int i = 0; i < n; i++)
      assert(compar[i] == vec[i]);
    vec.pop_back();
    int after = vec.capacity();
    if (before != after) {
      for (int i = 0; i < n - 1; i++)
        assert(compar[i] == vec[i]);
    }
  }
  std::print("\n");

  // deque
  ns::deque<int> deck(2);
  std::fill(deck.begin(), deck.end(), 4);
  deck.push_back(0);
  assert(deck.back() == 0);
  deck.push_back(1);
  assert(deck.back() == 1);
  deck.pop_back();
  deck.pop_back();
  assert(deck.capacity() == 4);
  assert(vec.size() == 2);
  for (int i = 0; i < num; ++i) {
    int before = deck.capacity();
    int n = deck.size();
    ns::deque<int> compar(deck);
    for (int i = 0; i < n; i++)
      assert(compar[i] == deck[i]);
    deck.push_back(std::rand() % 90 + 10);
    int after = deck.capacity();
    if (before != after) {
      for (int i = 0; i < n; i++)
        assert(compar[i] == deck[i]);
    }
  }
  std::print("\nDEQUE\t");
  std::for_each(deck.begin(), deck.end(),
                [](auto x) { std::print("{}\t", x); });

  for (int i = 0; i < num; ++i) {
    int before = deck.capacity();
    int n = deck.size();
    ns::deque<int> compar = deck;
    assert(compar.front() == deck.front());
    assert(compar.back() == deck.back());
    for (int i = 0; i < n; i++)
      assert(compar[i] == deck[i]);
    deck.pop_front();
    int after = deck.capacity();
    if (before != after) {
      for (int i = 1; i < n; i++)
        assert(compar[i] == deck[i - 1]);
    }
  }
  std::print("\n\n");

  // Graph
  constexpr int v{8}, e{12};
  Graph G(v);
  generateGraph(G, e);
  Cycle C(G);
  printCycle(C);
  std::print("\n");

  Digraph DG(v);
  generateGraph(DG, e);
  DirectedCycle DC(DG);
  printCycle(DC);
  std::print("\n");
}
