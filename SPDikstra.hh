#include "Graph.hh"
#include "PQ.hh"

struct DikstraSP {
  ns::deque<int> distTo;
  ns::deque<DirectedEdge *> edgeTo;
  IndexMinPQ<int> pq;

  DikstraSP(const EdgeWeightedDigraph &G, int s)
      : distTo(G.V, 0xffff), edgeTo(G.V, nullptr), pq(G.V) {
    assert(0 <= s && s < G.V);
    distTo[s] = 0;
    pq.insert(s, distTo[s]);
    while (!pq.empty()) {
      int v{pq.delMin()};
      for (const auto &e : G.adj[v]) relax(e);
    }
  }

  void relax(const DirectedEdge &e) {
    int v{e.from}, w{e.to};
    if (distTo[w] > distTo[v] + e.weight) {
      distTo[w] = distTo[v] + e.weight;
      delete edgeTo[w];
      edgeTo[w] = new DirectedEdge(e);
      if (pq.contains(w))
        pq.decreaseKey(w, distTo[w]);
      else
        pq.insert(w, distTo[w]);
    }
  }

  ~DikstraSP() {
    for (auto e : edgeTo) delete e;
  }

  bool hasPathTo(int v) const { return distTo[v] < 0xffff; }

  auto pathTo(int v) const {
    ns::deque<DirectedEdge> path;
    for (auto e{edgeTo[v]}; e; e = edgeTo[e->from]) path.push_front(*e);
    return path;
  }
};

// priority first search shortest path tree
struct LazyDikstra {
  struct Vertex {
    int v, distTo;
    bool operator>(const Vertex &rhs) const { return distTo > rhs.distTo; }
  };
  ns::deque<bool> marked;
  ns::deque<int> distTo;
  ns::deque<DirectedEdge *> edgeTo;
  PQ<Vertex, Bigger<Vertex>> pq;

  LazyDikstra(const EdgeWeightedDigraph &G, int s)
      : marked(G.V, false), distTo(G.V, 0xffff), edgeTo(G.V, nullptr), pq(G.E) {
    assert(0 <= s && s < G.V);
    distTo[s] = 0;
    pq.push({s, 0});
    while (!pq.empty()) {
      Vertex x{pq.pop()};
      int v{x.v};
      if (!marked[v]) relax(G, v);
    }
  }

  void relax(const EdgeWeightedDigraph &G, int v) {
    marked[v] = true;
    for (const auto &e : G.adj[v]) {
      int w{e.to};
      if (distTo[w] > distTo[v] + e.weight) {
        distTo[w] = distTo[v] + e.weight;
        delete edgeTo[w];
        edgeTo[w] = new DirectedEdge(e);
        pq.push({w, distTo[w]});
      }
    }
  }

  ~LazyDikstra() {
    for (auto e : edgeTo) delete e;
  }

  bool hasPathTo(int v) const { return distTo[v] < 0xffff; }

  auto pathTo(int v) const {
    ns::deque<DirectedEdge> path;
    for (auto e{edgeTo[v]}; e; e = edgeTo[e->from]) path.push_front(*e);
    return path;
  }
};
