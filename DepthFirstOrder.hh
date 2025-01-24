#pragma once
#include "Graph.hh"

struct DepthFirstOrder {
  ns::deque<bool> marked;
  ns::deque<int> preorder;
  ns::deque<int> postorder;

  DepthFirstOrder(const Digraph &G)
      : marked(G.V, false), preorder(), postorder() {
    for (int v = 0; v < G.V; ++v)
      if (!marked[v])
        dfs(G, v);
  }

  void dfs(const Digraph &G, int v) {
    preorder.push_back(v);
    marked[v] = true;
    for (int w : G.adj[v]) {
      if (!marked[w])
        dfs(G, w);
    }
    postorder.push_back(v);
  }

  DepthFirstOrder(const EdgeWeightedDigraph &G)
      : marked(G.V, false), preorder(), postorder() {
    for (int v = 0; v < G.V; ++v)
      if (!marked[v])
        dfs(G, v);
  }

  void dfs(const EdgeWeightedDigraph &G, int v) {
    preorder.push_back(v);
    marked[v] = true;
    for (const auto &e : G.adj[v]) {
      if (int w{e.to}; !marked[w])
        dfs(G, w);
    }
    postorder.push_back(v);
  }

  ns::deque<int> pre() const { return preorder; }
  ns::deque<int> post() const { return postorder; }
  ns::deque<int> reversePost() const {
    ns::deque<int> deck;
    for (int v : postorder)
      deck.push_front(v);
    return deck;
  }
};