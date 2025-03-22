#pragma once
#include <type_traits>

#include "Graph.hh"

struct Cycle {
  ns::deque<bool> marked;
  ns::deque<int> edgeTo;
  ns::deque<int> cycle;

  // no self cycle or parallel edge
  Cycle(const Graph &G) : marked(G.V, false), edgeTo(G.V, -1) {
    for (int v = 0; v < G.V; v++)
      if (!marked[v]) {
        dfs(G, -1, v);
        if (!cycle.empty()) return;
      }
  }

  void dfs(const Graph &G, int parent, int v) {
    marked[v] = true;
    for (int w : G.adj[v]) {
      if (!marked[w]) {
        edgeTo[w] = v;
        dfs(G, v, w);
        if (!cycle.empty()) return;
      } else if (w != parent) {
        for (int x = v; x != w; x = edgeTo[x]) cycle.push_back(x);
        cycle.push_back(w);
        cycle.push_back(v);
        return;
      }
    }
  }

  bool hasCycle() const { return !cycle.empty(); }
};

struct DirectedCycle {
  ns::deque<int> marked;
  ns::deque<int> onStack;
  ns::deque<int> edgeTo;
  ns::deque<int> cycle;

  DirectedCycle(const Digraph &G)
      : marked(G.V, false), onStack(G.V, false), edgeTo(G.V, -1) {
    for (int v = 0; v < G.V; ++v)
      if (!marked[v]) {
        dfs(G, v);
        if (!cycle.empty()) return;
      }
  }

  void dfs(const Digraph &G, int v) {
    onStack[v] = true;
    marked[v] = true;
    for (int w : G.adj[v]) {
      if (!marked[w]) {
        edgeTo[w] = v;
        dfs(G, w);
        if (!cycle.empty()) return;
      } else if (onStack[w]) {
        for (int x = v; x != w; x = edgeTo[x]) cycle.push_front(x);
        cycle.push_front(w);
        cycle.push_front(v);
        return;
      }
    }
    onStack[v] = false;
  }

  bool hasCycle() const { return !cycle.empty(); }
};

struct EdgeWeightedDirectedCycle {
  ns::deque<int> marked;
  ns::deque<int> onStack;
  ns::deque<DirectedEdge *> edgeTo;
  ns::deque<DirectedEdge> cycle;

  EdgeWeightedDirectedCycle(const EdgeWeightedDigraph &G)
      : marked(G.V, false), onStack(G.V, false), edgeTo(G.V, nullptr) {
    for (int v = 0; v < G.V; ++v)
      if (!marked[v]) {
        dfs(G, v);
        if (!cycle.empty()) return;
      }
  }

  void dfs(const EdgeWeightedDigraph &G, int v) {
    onStack[v] = true;
    marked[v] = true;
    for (const auto &e : G.adj[v]) {
      if (int w{e.to}; !marked[w]) {
        edgeTo[w] = new DirectedEdge(e);
        dfs(G, w);
        if (!cycle.empty()) return;
      } else if (onStack[w]) {
        DirectedEdge f(e);
        while (f.from != w) {
          cycle.push_back(f);
          f = *edgeTo[f.from];
        }
        cycle.push_back(f);
        return;
      }
    }
    onStack[v] = false;
  }

  ~EdgeWeightedDirectedCycle() {
    for (auto e : edgeTo) delete e;
  }

  bool hasCycle() const { return !cycle.empty(); }
};

template <class C>
void printCycle(const C &c) {
  if (c.hasCycle()) {
    std::print("Cycle\t");
    if constexpr (std::is_same_v<C, Cycle> || std::is_same_v<C, DirectedCycle>)
      for (auto cyc{c.cycle}; int v : cyc) std::print("{}\t", v);
    if constexpr (std::is_same_v<C, EdgeWeightedDirectedCycle>)
      for (auto cyc{c.cycle}; const auto &e : cyc)
        std::print("[{}->{}|{}]\t", e.from, e.to, e.weight);
  } else
    std::print("No Cycle");
  std::print("\n");
}