#include <cstdlib>
#include <ctime>
#include <deque>
#include <print>

// 零入度拓扑序算法
struct AdjMatrixDigraph {
  int V;
  std::deque<std::deque<bool>> adj;
  AdjMatrixDigraph(int v) : V{v}, adj(v, std::deque<bool>(v, false)) {}
};

struct TopologicalX {
  std::deque<int> topology;
  std::deque<int> rank;

  TopologicalX(const AdjMatrixDigraph &G) : rank(G.V, -1) {
    std::deque<int> indegree(G.V);
    int V = G.V;
    for (int i = 0; i < V; i++) {
      for (int j = 0; j < V; j++) {
        if (G.adj[i][j]) ++indegree[j];
      }
    }

    int count{0};
    std::deque<int> queue;
    for (int v = 0; v < V; ++v)
      if (indegree[v] == 0) queue.push_back(v);
    while (!queue.empty()) {
      int v = queue.front();
      queue.pop_front();
      topology.push_back(v);
      rank[v] = count++;
      for (int w = 0; w < V; ++w)
        if (G.adj[v][w]) {
          --indegree[w];
          if (indegree[w] == 0) queue.push_back(w);
        }
    }
    // clear if there is a cycle
    if (count != V) {
      topology.clear();
      rank = std::deque<int>(V, -1);
    }
  }

  bool hasOrder() const { return !topology.empty(); }
};

void printAdjMatrixDigraph(const AdjMatrixDigraph &G) {
  std::print("V\t");
  for (int i = 0; i < G.V; ++i) std::print("v{}\t", i);
  std::print("\n");
  for (int i = 0; i < G.V; ++i) {
    std::print("v{}\t", i);
    for (int j = 0; j < G.V; ++j) std::print("{}\t", G.adj[i][j]);
    std::print("\n");
  }
}

int main() {
  std::srand(std::time(nullptr));
  const int e{8}, v{8};
  AdjMatrixDigraph G(v);
  std::deque<int> a(e), b(e);
  int i{0};
  while (i < e) {
    a[i] = std::rand() % v, b[i] = std::rand() % v;
    if (a[i] == b[i]) continue;
    int j{0};
    for (; j < i; ++j) {
      if (a[i] == a[j] && b[i] == b[j]) break;
      if (a[i] == b[j] && b[i] == a[j]) break;
    }
    if (i != j) continue;
    G.adj[a[i]][b[i]] = true;
    ++i;
  }

  std::print("AdjMatrixDigraph\n");
  printAdjMatrixDigraph(G);
  std::print("\n");

  std::print("TopologicalX\t");
  std::deque<int> topology{TopologicalX(G).topology};
  for (const auto &e : topology) std::print("{}\t", e);
  std::print("\n");
}
