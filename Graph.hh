#pragma once
#include <cassert>
#include <print>
#include <random>
#include <type_traits>

#include "deque.hh"

struct Graph {
  int V, E;
  ns::deque<ns::deque<int>> adj;
  Graph(int v) : V{v}, E{0}, adj(v) {}
  void addEdge(int v, int w) {
    assert(0 <= v && v < V && 0 <= w && w < V);
    adj[v].push_back(w), adj[w].push_back(v);
    E++;
  }
};

struct Edge {
  int v, w, weight;
  Edge() {}
  Edge(int v, int w, int weight) : v{v}, w{w}, weight{weight} {}
  int other(int x) const {
    assert(x == v || x == w);
    return x == v ? w : v;
  }
  bool operator>(const Edge &rhs) const { return weight > rhs.weight; }
  bool operator<(const Edge &rhs) const { return weight < rhs.weight; }
  bool operator<=(const Edge &rhs) const { return weight <= rhs.weight; }
  bool operator>=(const Edge &rhs) const { return weight >= rhs.weight; }
  bool operator!=(const Edge &rhs) const { return weight != rhs.weight; }
  bool operator==(const Edge &rhs) const { return weight == rhs.weight; }
};

struct EdgeWeightedGraph {
  int V, E;
  ns::deque<ns::deque<Edge>> adj;
  EdgeWeightedGraph(int v) : V{v}, E{0}, adj(v) {}
  void addEdge(Edge e) {
    int v{e.v}, w{e.w};
    assert(0 <= v && v < V && 0 <= w && w < V);
    adj[v].push_back(e), adj[w].push_back(e);
    E++;
  }
  auto edges() const {
    ns::deque<Edge> deck;
    for (int v = 0; v < V; v++) {
      for (const auto &e : adj[v])
        if (e.other(v) > v) deck.push_back(e);
    }
    return deck;
  }
};

struct Digraph {
  int V, E;
  ns::deque<ns::deque<int>> adj;
  Digraph(int v) : V{v}, E{0}, adj(v) {}
  void addEdge(int v, int w) {
    assert(0 <= v && v < V && 0 <= w && w < V);
    adj[v].push_back(w);
    E++;
  }
  Digraph reverse() const {
    Digraph reverse(V);
    for (int v = 0; v < V; v++) {
      for (int w : adj[v]) reverse.addEdge(w, v);
    }
    return reverse;
  }
};

struct DirectedEdge {
  int from, to, weight;
  DirectedEdge() {}
  DirectedEdge(int from, int to, int weight)
      : from{from}, to{to}, weight{weight} {}
  bool operator>(const DirectedEdge &r) const { return weight > r.weight; }
  bool operator<(const DirectedEdge &r) const { return weight < r.weight; }
  bool operator<=(const DirectedEdge &r) const { return weight <= r.weight; }
  bool operator>=(const DirectedEdge &r) const { return weight >= r.weight; }
  bool operator!=(const DirectedEdge &r) const { return weight != r.weight; }
  bool operator==(const DirectedEdge &r) const { return weight == r.weight; }
};

struct EdgeWeightedDigraph {
  int V, E;
  ns::deque<ns::deque<DirectedEdge>> adj;
  EdgeWeightedDigraph(int v) : V{v}, E{0}, adj(v) {}
  void addEdge(DirectedEdge e) {
    assert(0 <= e.from && e.from < V);
    assert(0 <= e.to && e.to < V);
    adj[e.from].push_back(e);
    E++;
  }
  EdgeWeightedDigraph reverse() const {
    EdgeWeightedDigraph reverse(V);
    for (int v = 0; v < V; v++) {
      for (const auto &e : adj[v]) reverse.addEdge({e.to, e.from, e.weight});
    }
    return reverse;
  }
  auto edges() const {
    ns::deque<DirectedEdge> deck;
    for (int v = 0; v < V; v++) {
      for (const auto &e : adj[v]) deck.push_back(e);
    }
    return deck;
  }
};

// one generator to rule them all
#ifdef __cpp_lib_concepts
template <class T>
concept isGraphType = std::is_same_v<T, Graph> || std::is_same_v<T, Digraph> or
                      std::is_same_v<T, EdgeWeightedGraph> or
                      std::is_same_v<T, EdgeWeightedDigraph>;
#endif
template <class T>
void generateGraph(T &G, int e)
#ifdef __cpp_lib_concepts
  requires isGraphType<T>
#endif
{
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution randV(0, G.V - 1), randE(0, 7);
  ns::deque<int> a(e), b(e);
  int i{0};
  while (i < e) {
    a[i] = randV(mt), b[i] = randV(mt);
    // no self cycle
    if (a[i] == b[i]) continue;
    int j{0};
    for (; j < i; ++j) {
      // no parallel edge
      if (a[i] == a[j] && b[i] == b[j]) break;
      // no cycle with two vertex
      if (a[i] == b[j] && b[i] == a[j]) break;
    }
    if (i != j) continue;
    if constexpr (std::is_same_v<T, Graph> || std::is_same_v<T, Digraph>)
      G.addEdge(a[i], b[i]);
    else if constexpr (std::is_same_v<T, EdgeWeightedGraph>)
      G.addEdge({a[i], b[i], randE(mt)});
    else if constexpr (std::is_same_v<T, EdgeWeightedDigraph>)
      G.addEdge({a[i], b[i], randE(mt)});
    ++i;
  }
}

// one printer to rule them all
template <class T>
void printGraph(T &&G)
#ifdef __cpp_lib_concepts
  requires isGraphType<std::remove_reference_t<T>>
#endif
{
  for (int i = 0; i < G.V; ++i) {
    std::print("v{}\t", i);
    for (const auto &e : G.adj[i]) {
      if constexpr (std::is_same_v<std::remove_reference_t<T>, Graph> or
                    std::is_same_v<std::remove_reference_t<T>, Digraph>)
        std::print("[{}<->{}]\t", i, e);
      else if constexpr (std::is_same_v<std::remove_reference_t<T>,
                                        EdgeWeightedGraph>)
        std::print("[{}->{}|{:+}]\t", e.v, e.w, e.weight);
      else if constexpr (std::is_same_v<std::remove_reference_t<T>,
                                        EdgeWeightedDigraph>)
        std::print("[{}->{}|{:+}]\t", e.from, e.to, e.weight);
    }
    std::print("\n");
  }
}