#include "Graph.hh"

// 图连通性
struct CC {
  ns::deque<bool> marked;
  ns::deque<int> id;
  int count;

  CC(const Graph &G) : marked(G.V, false), id(G.V), count{0} {
    for (int v = 0; v < G.V; ++v)
      if (!marked[v]) {
        bfs(G, v);
        ++count;
      }
  }

  void dfs(const Graph &G, int v) {
    // 首次发现时标记顶点
    marked[v] = true;
    id[v] = count;
    for (int w : G.adj[v]) {
      if (!marked[w]) dfs(G, w);
    }
  }

  void bfs(const Graph &G, int s) {
    // 首次发现时标记顶点
    marked[s] = true;
    id[s] = count;
    ns::deque<int> queue;
    queue.push_back(s);
    while (!queue.empty()) {
      int v{queue.front()};
      queue.pop_front();
      for (int w : G.adj[v]) {
        if (!marked[w]) {
          marked[w] = true;
          id[w] = count;
          queue.push_back(w);
        }
      }
    }
  }

  bool connected(int v, int w) const { return id[v] == id[w]; }
};

template <class CC>
void printCC(const CC &cc, int v) {
  std::print("vertex\t");
  for (int i = 0; i < v; ++i) std::print("v{}\t", i);
  std::print("\n");
  std::print("id\t");
  for (int i = 0; i < v; ++i) std::print("{}\t", cc.id[i]);
  std::print("\n");
}

struct Articulation {
  int clock{0};
  ns::deque<int> dTime, low;
  ns::deque<bool> visited, isArticulation;

  Articulation(const Graph &G)
      : dTime(G.V), low(G.V), visited(G.V, false), isArticulation(G.V, false) {
    for (int v = 0; v < G.V; v++) {
      if (!visited[v]) dfs(G, v, -1);
    }
  }

  void dfs(const Graph &G, int u, int parent) {
    int child{0};
    low[u] = dTime[u] = ++clock;
    visited[u] = true;
    for (int v : G.adj[u]) {
      if (!visited[v]) {
        child++;
        dfs(G, v, u);
        // update low when child's dfs return
        low[u] = std::min(low[u], low[v]);
        if (parent != -1 && low[v] >= dTime[u]) isArticulation[u] = true;
      } else if (v != parent) {
        // update low when there is a backward edge
        low[u] = std::min(low[u], dTime[v]);
      }
    }
    // bottom vertex of dfs stack
    if (parent == -1 && child > 1) isArticulation[u] = true;
  }
};

int main() {
  constexpr int v{16}, e{16};
  for (int cnt = 0; cnt < 64; cnt++) {
    Graph G(v);
    generateGraph(G, e);
    std::print("Graph\n");
    printGraph(G);

    std::print("\nConnectedComponet\n");
    CC cc(G);
    printCC(cc, G.V);
    std::print("\n");

    Articulation A(G);
    std::print("vertex\t");
    for (int i = 0; i < v; i++) std::print("{}\t", i);
    std::print("\n");
    std::print("isCut\t");
    for (int i = 0; i < v; i++) std::print("{}\t", A.isArticulation[i]);
    std::print("\n");

    for (int i = 0; i < v; i++) {
      Graph H(v);
      for (int j = 0; j < v; j++) {
        if (j != i) {
          for (int k : G.adj[j]) {
            if (k != i && k < j) H.addEdge(j, k);
          }
        }
      }
      assert(H.E == G.E - G.adj[i].size());
      if (A.isArticulation[i]) {
        assert(CC(H).count > cc.count);
      } else if (G.adj[i].size() > 0) {
        assert(CC(H).count == cc.count + 1);
      } else {
        assert(CC(H).count == cc.count);
      }
    }
  }
}
