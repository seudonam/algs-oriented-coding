#include <cassert>
#include <deque>
#include <memory>
#include <print>

struct FlowEdge {
  int from, to, flow, cap;

  FlowEdge(int from, int to, int cap) : from{from}, to{to}, flow{0}, cap{cap} {}

  int other(int vertex) {
    assert(vertex == from || vertex == to);
    if (vertex == from)
      return to;
    else
      return from;
  }

  int residualCapacityTo(int vertex) {
    assert(vertex == from || vertex == to);
    if (vertex == from)
      return flow;
    else
      return cap - flow;
  }

  void addResidualFlowTo(int vertex, int delta) {
    assert(vertex == from || vertex == to);
    if (vertex == from)
      flow -= delta;
    else
      flow += delta;
  }
};

template <class T> using PtrList = std::deque<std::shared_ptr<T>>;

struct FlowNetwork {
  int V;
  std::deque<PtrList<FlowEdge>> adj;
  FlowNetwork(int v) : V{v}, adj(V) {}

  void addEdge(int from, int to, int cap) {
    auto e{std::make_shared<FlowEdge>(from, to, cap)};
    adj[e->from].push_back(e);
    adj[e->to].push_back(e);
  }
};

struct FordFulkerson {
  std::deque<bool> marked;
  PtrList<FlowEdge> edgeTo;
  int value{0};

  FordFulkerson(FlowNetwork &G, int s, int t) {
    while (hasAugmentingPath(G, s, t)) {
      int bottle{0xffff};
      for (int v{t}; v != s; v = edgeTo[v]->other(v))
        bottle = std::min(bottle, edgeTo[v]->residualCapacityTo(v));
      for (int v{t}; v != s; v = edgeTo[v]->other(v))
        edgeTo[v]->addResidualFlowTo(v, bottle);
      value += bottle;
    }
  }

  bool hasAugmentingPath(FlowNetwork &G, int s, int t) {
    edgeTo = PtrList<FlowEdge>(G.V, nullptr);
    marked = std::deque<bool>(G.V, false);
    std::deque<int> queue;
    queue.push_back(s);
    marked[s] = true;
    while (!queue.empty()) {
      int v{queue.front()};
      queue.pop_front();
      for (const auto &e : G.adj[v]) {
        int w{e->other(v)};
        if (!marked[w] && (e->residualCapacityTo(w) > 0)) {
          edgeTo[w] = e;
          marked[w] = true;
          queue.push_back(w);
        }
      }
    }
    return marked[t];
  }
};

int main() {
  FlowNetwork G(6);
  G.addEdge(0, 1, 2);
  G.addEdge(0, 2, 3);
  G.addEdge(1, 3, 3);
  G.addEdge(1, 4, 1);
  G.addEdge(2, 3, 1);
  G.addEdge(2, 4, 1);
  G.addEdge(3, 5, 2);
  G.addEdge(4, 5, 3);
  int s = 0, t = G.V - 1;
  FordFulkerson maxflow(G, s, t);
  std::print("Max flow from {} to {}\n", s, t);
  for (int v = 0; v < G.V; v++)
    for (const auto &e : G.adj[v])
      if (v == e->from && e->flow > 0)
        std::print(" {}->{} {} {}\n", e->from, e->to, e->cap, e->flow);
  std::print("Max flow value = {}\n", maxflow.value);
}