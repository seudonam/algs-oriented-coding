#include "Graph.hh"

struct Bipartite {
  bool bipartite{true};
  ns::deque<bool> color;
  ns::deque<bool> marked;
  ns::deque<int> edgeTo;
  ns::deque<int> cycle;

  Bipartite(const Graph &G)
      : color(G.V, false), marked(G.V, false), edgeTo(G.V, -1) {
    for (int v = 0; v < G.V; v++)
      if (!marked[v]) {
        dfs(G, v);
        if (!cycle.empty()) return;
      }
  }

  void dfs(const Graph &G, int v) {
    marked[v] = true;
    for (int w : G.adj[v]) {
      if (!marked[w]) {
        edgeTo[w] = v;
        color[w] = !color[v];
        dfs(G, w);
        if (!cycle.empty()) return;
      } else if (color[w] == color[v]) {
        bipartite = false;
        cycle.push_back(w);
        for (int x = v; x != w; x = edgeTo[x]) {
          cycle.push_back(x);
        }
        cycle.push_back(w);
        return;
      }
    }
  }

  bool isBipartite() { return !cycle.empty(); }
};

int main() {
  constexpr int v{4}, e{4};
  std::print("Graph\n");
  Graph G(v);
  generateGraph(G, e);
  printGraph(G);

  Bipartite bp(G);
  if (bp.bipartite) {
    std::print("IsBipartite");
    for (int i = 0; i < v; i++) {
      for (int w : G.adj[i]) assert(bp.color[w] == !bp.color[i]);
    }
  } else {
    std::print("cycle\t");
    for (int v : bp.cycle) std::print("{}\t", v);
  }
  std::print("\n");
}