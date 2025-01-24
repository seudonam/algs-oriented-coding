#include "deque.hh"
#include <cstdlib>
#include <ctime>
#include <print>

using AdjList = ns::deque<ns::deque<int>>;

struct DepthFirstSearch {
  enum class genre { UNDISCOVERED, DISCOVERED, VISITED };
  int clock{0};
  ns::deque<genre> status;
  ns::deque<int> dTime, fTime;

  DepthFirstSearch(const AdjList &adj, int src)
      : status(adj.size(), genre::UNDISCOVERED), dTime(adj.size()),
        fTime(adj.size()) {
    dfs(adj, src);
  }

  // Tree Edge: DISCOVERED v  to UNDISCOVERED w
  // Backward Edge: DISCOVERED v to DISCOVERED w
  // Forward Edge: DISCOVERED v to VISITED w && dTime(v) < dTime(w)
  // Cross Edge: DISCOVERED v to VISITED w && dTime(v) > dTime(w)
  void dfs(const AdjList &adj, int v) {
    dTime[v] = ++clock;
    status[v] = genre::DISCOVERED;
    for (int w : adj[v])
      switch (status[w]) {
      case genre::UNDISCOVERED:
        dfs(adj, w);
        break;
      case genre::DISCOVERED:
        break;
      case genre::VISITED:
        break;
      }
    status[v] = genre::VISITED;
    fTime[v] = clock++;
  }
};

int main() {
  constexpr int v{8}, e{16};
  std::srand(std::time(nullptr));
  AdjList G(v);
  ns::deque<int> a(e), b(e);
  int i{0};
  while (i < e) {
    a[i] = std::rand() % v, b[i] = std::rand() % v;
    if (a[i] == b[i])
      continue;
    int j{0};
    for (; j < i; ++j) {
      if (a[i] == a[j] && b[i] == b[j])
        break;
      if (a[i] == b[j] && b[i] == a[j])
        break;
    }
    if (i != j)
      continue;
    G[a[i]].push_back(b[i]);
    ++i;
  }
  DepthFirstSearch DFS(G, 0);
  std::print("vertex\t");
  for (int i = 0; i < v; ++i)
    std::print("v{}\t", i);
  std::print("\n");
  std::print("dTime\t");
  for (int v : DFS.dTime)
    std::print("{}\t", v);
  std::print("\n");
  std::print("fTime\t");
  for (int v : DFS.fTime)
    std::print("{}\t", v);
  std::print("\n");
}