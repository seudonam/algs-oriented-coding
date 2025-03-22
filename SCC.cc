#include "DepthFirstOrder.hh"
#include "Graph.hh"

struct KosarajuSCC {
  ns::deque<bool> marked;
  ns::deque<int> id;
  int count;

  KosarajuSCC(const Digraph &G) : marked(G.V, false), id(G.V), count{0} {
    DepthFirstOrder DFS(G.reverse());
    for (int v : DFS.reversePost())
      if (!marked[v]) {
        dfs(G, v);
        ++count;
      }
  }

  void dfs(const Digraph &G, int v) {
    marked[v] = true;
    id[v] = count;
    for (int w : G.adj[v]) {
      if (!marked[w]) dfs(G, w);
    }
  }

  bool connected(int v, int w) const { return id[v] == id[w]; }
};

struct TarjanSCC {
  ns::deque<bool> marked;
  ns::deque<int> id, low, stack;
  int clock{0}, count{0};

  TarjanSCC(const Digraph &G) : marked(G.V, false), id(G.V), low(G.V) {
    for (int v = 0; v < G.V; v++)
      if (!marked[v]) dfs(G, v);
  }

  void dfs(const Digraph &G, int v) {
    marked[v] = true;
    low[v] = ++clock;
    int min = low[v];
    stack.push_back(v);
    for (int w : G.adj[v]) {
      if (!marked[w]) dfs(G, w);
      if (low[w] < min) min = low[w];
    }
    if (min < low[v]) {
      low[v] = min;
      return;
    }
    int w;
    do {
      w = stack.back();
      stack.pop_back();
      id[w] = count;
      low[w] = G.V;
    } while (w != v);
    count++;
  }

  bool connected(int v, int w) const { return id[v] == id[w]; }
};

template <class SCC>
void printSCC(const SCC &scc, int v) {
  std::print("vertex\t");
  for (int i = 0; i < v; ++i) std::print("v{}\t", i);
  std::print("\n");
  std::print("id\t");
  for (int i = 0; i < v; ++i) std::print("{}\t", scc.id[i]);
  std::print("\n");
}

int main() {
  constexpr int v{16}, e{16};
  for (int cnt = 0; cnt < 64; cnt++) {
    std::print("Graph\n");
    Graph G(v);
    generateGraph(G, e);
    printGraph(G);

    std::print("\nDigraph\n");
    Digraph DG(v);
    generateGraph(DG, e);
    printGraph(DG);

    std::print("\nKosarajuSCC\n");
    KosarajuSCC kscc(DG);
    printSCC(kscc, G.V);

    std::print("\nTarjanSCC\n");
    TarjanSCC tscc(DG);
    printSCC(tscc, G.V);
    assert(kscc.count == tscc.count);
    for (int v = 0; v < G.V; v++)
      for (int w = v; w < G.V; w++)
        assert(tscc.connected(v, w) == kscc.connected(v, w));
  }
}