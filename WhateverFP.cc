#include "Graph.hh"

// 图可达性
struct DepthFirstPaths {
  ns::deque<bool> marked;
  ns::deque<int> edgeTo;
  int s;
  DepthFirstPaths(const Graph &G, int s)
      : marked(G.V, false), edgeTo(G.V, -1), s{s} {
    assert(0 <= s && s < G.V);
    dfs(G, s);
  }
  // 递归dfs
  void dfs(const Graph &G, int v) {
    marked[v] = true;
    for (int w : G.adj[v]) {
      if (!marked[w]) {
        edgeTo[w] = v;
        dfs(G, w);
      }
    }
  }

  DepthFirstPaths(const Digraph &G, int s)
      : marked(G.V, false), edgeTo(G.V, -1), s{s} {
    assert(0 <= s && s < G.V);
    dfs(G, s);
  }
  // 迭代dfs
  void dfs(const Digraph &G, int s) {
    marked[s] = true;
    ns::deque<int> stack;
    stack.push_back(s);
    while (!stack.empty()) {
      int v = stack.back();
      stack.pop_back();
      for (int w : G.adj[v])
        if (!marked[w]) {
          marked[w] = true;
          edgeTo[w] = v;
          stack.push_back(w);
        }
    }
  }
  bool hasPathTo(int v) const { return marked[v]; }
  ns::deque<int> pathTo(int v) const {
    ns::deque<int> path;
    if (hasPathTo(v)) {
      for (int x = v; x != s; x = edgeTo[x])
        path.push_front(x);
      path.push_front(s);
    }
    return path;
  }
};

struct BreadthFirstPaths {
  ns::deque<bool> marked;
  ns::deque<int> edgeTo, distTo;
  int s;
  BreadthFirstPaths(const Graph &G, int s)
      : marked(G.V, false), edgeTo(G.V, -1), distTo(G.V, 0xff), s{s} {
    assert(0 <= s && s < G.V);
    distTo[s] = 0;
    bfs(G, s);
  }
  // 第一类bfs
  void bfs(const Graph &G, int s) {
    marked[s] = true;
    ns::deque<int> queue;
    queue.push_back(s);
    while (!queue.empty()) {
      int v = queue.front();
      queue.pop_front();
      for (int w : G.adj[v])
        if (!marked[w]) {
          marked[w] = true;
          edgeTo[w] = v;
          distTo[w] = distTo[v] + 1;
          queue.push_back(w);
        }
    }
  }
  BreadthFirstPaths(const Digraph &G, int s)
      : marked(G.V, false), edgeTo(G.V, -1), distTo(G.V, 0xff), s{s} {
    assert(0 <= s && s < G.V);
    distTo[s] = 0;
    bfs(G, s);
  }
  // 第二类bfs
  void bfs(const Digraph &G, int s) {
    marked[s] = true;
    ns::deque<int> queue;
    queue.push_back(s);
    while (!queue.empty()) {
      for (int i(0), sz(queue.size()); i < sz; ++i) {
        int v = queue.front();
        queue.pop_front();
        for (int w : G.adj[v])
          if (!marked[w]) {
            marked[w] = true;
            edgeTo[w] = v;
            distTo[w] = distTo[v] + 1;
            queue.push_back(w);
          }
      }
    }
  }
  bool hasPathTo(int v) const { return marked[v]; }
  ns::deque<int> pathTo(int v) const {
    ns::deque<int> path;
    if (hasPathTo(v)) {
      for (int x = v; x != s; x = edgeTo[x])
        path.push_front(x);
      path.push_front(s);
    }
    return path;
  }
};

template <class P> void printPath(const P &p, int v) {
  for (int i = 0; i < v; ++i) {
    std::print("v{}\t", i);
    if (p.hasPathTo(i)) {
      auto path{p.pathTo(i)};
      for (int v : path)
        std::print("{}\t", v);
    } else
      std::print("No Path");
    std::print("\n");
  }
}

int main() {
  constexpr int v{8}, e{12};
  Digraph DIG(v);
  generateGraph(DIG, e);
  printGraph(DIG);
  std::print("\n");

  std::print("dfs\n");
  DepthFirstPaths dfs(DIG, 0);
  printPath(dfs, v);
  std::print("\n");

  std::print("bfs\n");
  BreadthFirstPaths bfs(DIG, 0);
  printPath(bfs, v);
  std::print("vertex\t");
  for (int i = 0; i < v; ++i)
    std::print("v{}\t", i);
  std::print("\n");
  std::print("distTo\t");
  for (int v : bfs.distTo)
    std::print("{}\t", v);
  std::print("\n");
}
