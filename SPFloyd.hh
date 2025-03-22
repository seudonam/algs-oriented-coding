#include "deque.hh"

struct AdjMatrixEdgeWeightedDigraph {
  int V;
  ns::deque<ns::deque<int>> adj;
  AdjMatrixEdgeWeightedDigraph(int v)
      : V{v}, adj(v, ns::deque<int>(v, 0xffff)) {}
};

struct Floyd {
  bool negativeCycle;
  ns::deque<ns::deque<int>> distTo;

  Floyd(const AdjMatrixEdgeWeightedDigraph &G)
      : negativeCycle{false}, distTo{G.adj} {
    int V = G.V;
    for (int v = 0; v < V; ++v) distTo[v][v] = 0;

    for (int i = 0; i < V; ++i) {
      for (int v = 0; v < V; ++v) {
        if (distTo[v][i] == 0xffff) continue;
        for (int w = 0; w < V; ++w) {
          if (distTo[i][w] == 0xffff) continue;
          if (distTo[v][w] > distTo[v][i] + distTo[i][w])
            distTo[v][w] = distTo[v][i] + distTo[i][w];
        }
        if (distTo[v][v] < 0) {
          negativeCycle = true;
          return;
        }
      }
    }
  }
};
