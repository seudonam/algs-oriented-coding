#include "Cycle.hh"
#include "DepthFirstOrder.hh"
#include "Graph.hh"

struct AcyclicSP {
  ns::deque<DirectedEdge *> edgeTo;
  ns::deque<int> distTo;

  AcyclicSP(const EdgeWeightedDigraph &G, int s)
      : edgeTo(G.V, nullptr), distTo(G.V, 0xffff) {
    assert(0 <= s && s < G.V);
    assert(!EdgeWeightedDirectedCycle(G).hasCycle());
    distTo[s] = 0;
    ns::deque<int> topological{DepthFirstOrder(G).reversePost()};
    for (int v : topological) {
      if (distTo[v] == 0xffff)
        continue;
      for (const auto &e : G.adj[v])
        relax(e);
    }
  }

  void relax(const DirectedEdge &e) {
    int v{e.from}, w{e.to};
    if (distTo[w] > distTo[v] + e.weight) {
      distTo[w] = distTo[v] + e.weight;
      delete edgeTo[w];
      edgeTo[w] = new DirectedEdge(e);
    }
  }

  ~AcyclicSP() {
    for (auto e : edgeTo)
      delete e;
  }

  bool hasPathTo(int v) const { return distTo[v] < 0xffff; }
  auto pathTo(int v) const {
    ns::deque<DirectedEdge> path;
    for (auto e{edgeTo[v]}; e; e = edgeTo[e->from])
      path.push_front(*e);
    return path;
  }
};

struct AcyclicSPX {
  ns::deque<int> distTo;

  AcyclicSPX(const EdgeWeightedDigraph &G, int s) : distTo(G.V, 0xffff) {
    assert(0 <= s && s < G.V);
    assert(!EdgeWeightedDirectedCycle(G).hasCycle());
    EdgeWeightedDigraph reverse{G.reverse()};
    ns::deque<int> aux(G.V, 0x7fffffff);
    for (int v = 0; v < G.V; ++v)
      distTo[v] = search(reverse, aux, s, v);
  }

  int search(const EdgeWeightedDigraph &G, ns::deque<int> &aux, int src,
             int dst) const {
    if (src == dst)
      return 0;
    else if (G.adj[dst].empty())
      return 0xffff;
    else if (aux[dst] < 0x7fffffff)
      return aux[dst];
    else {
      int x{0xffff};
      for (const auto &e : G.adj[dst]) {
        int y{search(G, aux, src, e.to)};
        if (y < 0xffff)
          x = std::min(x, e.weight + y);
      }
      aux[dst] = x;
      return x;
    }
  }
};
