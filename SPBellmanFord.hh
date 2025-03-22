#include "Cycle.hh"
#include "Graph.hh"

struct BellmanFord {
  ns::deque<DirectedEdge *> edgeTo;
  ns::deque<int> distTo;
  bool negativeCycle;

  BellmanFord(const EdgeWeightedDigraph &G, int s)
      : edgeTo(G.V, nullptr), distTo(G.V, 0xffff), negativeCycle{false} {
    assert(0 <= s && s < G.V);
    distTo[s] = 0;
    for (int pass = 1; pass < G.V; ++pass)
      for (int v = 0; v < G.V; ++v) {
        if (distTo[v] == 0xffff) continue;
        for (const auto &e : G.adj[v]) {
          int w{e.to};
          if (distTo[w] > distTo[v] + e.weight) {
            distTo[w] = distTo[v] + e.weight;
            delete edgeTo[w];
            edgeTo[w] = new DirectedEdge(e);
          }
        }
      }
    // check negative cycle
    for (int v = 0; v < G.V; ++v) {
      if (distTo[v] == 0xffff) continue;
      for (const auto &e : G.adj[v]) {
        int v{e.from}, w{e.to};
        if (distTo[w] > distTo[v] + e.weight) {
          negativeCycle = true;
          return;
        }
      }
    }
  }

  ~BellmanFord() {
    for (auto e : edgeTo) delete e;
  }

  bool hasNegativeCycle() const { return negativeCycle; }

  bool hasPathTo(int v) const {
    return !hasNegativeCycle() && distTo[v] < 0xffff;
  }

  auto pathTo(int v) const {
    ns::deque<DirectedEdge> path;
    for (auto e{edgeTo[v]}; e; e = edgeTo[e->from]) path.push_front(*e);
    return path;
  }
};

struct QueueBasedBF {
  ns::deque<int> distTo;
  ns::deque<DirectedEdge *> edgeTo;
  ns::deque<bool> onQueue;
  ns::deque<int> queue;
  ns::deque<DirectedEdge> cycle;
  int cost;

  QueueBasedBF(const EdgeWeightedDigraph &G, int s)
      : distTo(G.V, 0xffff),
        edgeTo(G.V, nullptr),
        onQueue(G.V, false),
        cost{0} {
    assert(0 <= s && s < G.V);
    distTo[s] = 0;
    queue.push_back(s), onQueue[s] = true;
    while (!queue.empty() && !hasNegativeCycle()) {
      int v{queue.front()};
      queue.pop_front();
      onQueue[v] = false;
      relax(G, v);
    }
  }

  void relax(const EdgeWeightedDigraph &G, int v) {
    for (const auto &e : G.adj[v]) {
      int w{e.to};
      if (distTo[w] > distTo[v] + e.weight) {
        distTo[w] = distTo[v] + e.weight;
        delete edgeTo[w];
        edgeTo[w] = new DirectedEdge(e);
        if (!onQueue[w]) {
          queue.push_back(w);
          onQueue[w] = true;
        }
      }
      // check negative cycle
      if (++cost % G.V == 0) {
        findNegativeCycle();
        if (hasNegativeCycle()) return;
      }
    }
  }

  ~QueueBasedBF() {
    for (auto e : edgeTo) delete e;
  }

  void findNegativeCycle() {
    int V{edgeTo.size()};
    EdgeWeightedDigraph spt(V);
    for (int v = 0; v < V; ++v) {
      if (edgeTo[v]) spt.addEdge(*edgeTo[v]);
    }
    EdgeWeightedDirectedCycle finder(spt);
    if (finder.hasCycle()) cycle = ns::deque<DirectedEdge>(finder.cycle);
  }

  bool hasNegativeCycle() const { return !cycle.empty(); }

  ns::deque<DirectedEdge> negativeCycle() const { return cycle; }

  bool hasPathTo(int v) const {
    return !hasNegativeCycle() && distTo[v] < 0xffff;
  }
  auto pathTo(int v) const {
    ns::deque<DirectedEdge> path;
    for (auto e{edgeTo[v]}; e; e = edgeTo[e->from]) path.push_front(*e);
    return path;
  }
};