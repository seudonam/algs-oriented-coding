#include <cassert>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <functional>
#include <memory>
#include <print>
#include <queue>

template <class T> using adj_list = std::deque<std::deque<T>>;

struct directed_edge {
  int from, to, weight;
};

struct dikstra {
  struct vertex {
    int v, dist_to;
    bool operator>(const vertex &rhs) const { return dist_to > rhs.dist_to; }
  };
  std::deque<bool> marked;
  std::deque<int> dist_to;
  std::deque<std::shared_ptr<directed_edge>> edge_to;
  std::priority_queue<vertex, std::deque<vertex>, std::greater<vertex>> pq;
  dikstra(const adj_list<directed_edge> &graph, int src)
      : marked(graph.size(), false), dist_to(graph.size(), 0xffff),
        edge_to(graph.size(), nullptr) {
    dist_to[src] = 0;
    pq.push({src, 0});
    while (!pq.empty()) {
      vertex x{pq.top()};
      pq.pop();
      int v{x.v};
      if (!marked[v])
        relax(graph, v);
    }
  }
  void relax(const adj_list<directed_edge> &graph, int v) {
    marked[v] = true;
    for (const auto &e : graph[v]) {
      int w{e.to};
      if (dist_to[w] > dist_to[v] + e.weight) {
        dist_to[w] = dist_to[v] + e.weight;
        edge_to[w] = std::make_shared<directed_edge>(e);
        pq.push({w, dist_to[w]});
      }
    }
  }
  bool has_path_to(int v) const { return dist_to[v] < 0xffff; }
  auto path_to(int v) const {
    std::deque<directed_edge> path;
    for (auto e{edge_to[v]}; e; e = edge_to[e->from])
      path.push_front(*e);
    return path;
  }
};

struct naive_dikstra {
  std::deque<bool> marked;
  std::deque<int> dist_to;
  naive_dikstra(const adj_list<directed_edge> &graph, int src)
      : marked(graph.size(), false), dist_to(graph.size(), 0xffff) {
    dist_to[src] = 0;
    for (int k = 0; k < graph.size(); k++) {
      int x{-1}, min{0xffff};
      for (int v = 0; v < graph.size(); v++) {
        if (!marked[v] && dist_to[v] < min) {
          x = v;
          min = dist_to[v];
        }
      }
      relax(graph, x);
    }
  }
  void relax(const adj_list<directed_edge> &graph, int v) {
    marked[v] = true;
    for (const auto &e : graph[v]) {
      int w{e.to};
      if (dist_to[w] > dist_to[v] + e.weight)
        dist_to[w] = dist_to[v] + e.weight;
    }
  }
};

struct edge {
  int v, w, weight;
  int other(int x) const { return x == v ? w : v; }
  bool operator>(const edge &rhs) const { return weight > rhs.weight; }
};

struct prim {
  int weight;
  std::deque<bool> marked;
  std::deque<edge> mst;
  std::priority_queue<edge, std::deque<edge>, std::greater<edge>> pq;

  prim(const adj_list<edge> &graph) : weight{0}, marked(graph.size(), false) {
    for (int v = 0; v < graph.size(); ++v)
      if (!marked[v])
        search(graph, v);
  }
  void search(const adj_list<edge> &graph, int s) {
    scan(graph, s);
    while (!pq.empty()) {
      edge e{pq.top()};
      pq.pop();
      int v{e.v}, w{e.w};
      if (marked[v] && marked[w])
        continue;
      mst.push_back(e), weight += e.weight;
      if (!marked[v])
        scan(graph, v);
      if (!marked[w])
        scan(graph, w);
    }
  }
  void scan(const adj_list<edge> &graph, int v) {
    marked[v] = true;
    for (const auto &e : graph[v])
      if (!marked[e.other(v)])
        pq.push(e);
  }
};

struct kruskal {
  int weight;
  std::deque<edge> mst;
  std::deque<int> parent;

  // union find
  int find(int v) {
    if (v != parent[v])
      parent[v] = find(parent[v]);
    return parent[v];
  }
  void join(int x, int y) { parent[x] = y; }

  kruskal(const std::deque<edge> &graph, int num) : weight{0}, parent(num) {
    for (int v = 0; v < num; ++v)
      parent[v] = v;
    std::priority_queue<edge, std::deque<edge>, std::greater<edge>> pq(
        graph.begin(), graph.end());
    while (!pq.empty() && mst.size() < num - 1) {
      edge e{pq.top()};
      pq.pop();
      int root_v{find(e.v)}, root_w{find(e.w)};
      if (root_v != root_w) {
        join(root_v, root_w);
        mst.push_back(e), weight += e.weight;
      }
    }
  }
};

int main() {
  std::srand(std::time(nullptr));
  constexpr int v{8}, e{16};
  for (int k = 0; k < 16; k++) {
    adj_list<edge> ewg(v);
    adj_list<directed_edge> ewd(v);
    std::deque<edge> edge_list;
    std::deque<int> a(e), b(e);
    int i{0};
    while (i < e) {
      a[i] = std::rand() % v, b[i] = std::rand() % v;
      if (a[i] == b[i])
        continue;
      int j{0};
      for (; j < i; ++j) {
        if (a[i] == a[j] && b[i] == b[j])
          break;
        if (a[i] == b[j] && b[i] == a[j])
          break;
      }
      if (i != j)
        continue;
      edge x{a[i], b[i], std::rand() % 8};
      edge_list.push_back(x);
      ewg[a[i]].push_back(x), ewg[b[i]].push_back(x);
      ewd[a[i]].push_back({a[i], b[i], std::rand() % 8});
      ++i;
    }

    dikstra dsp(ewd, 0);
    for (int i(0), sz(ewd.size()); i < sz; ++i) {
      std::print("v{}\t", i);
      if (dsp.has_path_to(i)) {
        auto path{dsp.path_to(i)};
        for (const auto &e : path)
          std::print("[{}->{}|{}]\t", e.from, e.to, e.weight);
      } else {
        std::print("No Path");
      }
      std::print("\n");
    }
    naive_dikstra ndsp(ewd, 0);
    assert(std::equal(dsp.dist_to.begin(), dsp.dist_to.end(),
                      ndsp.dist_to.begin(), ndsp.dist_to.end()));
    std::print("\n");

    kruskal kmst(edge_list, v);
    auto klist{kmst.mst};
    for (const auto &e : klist)
      std::print("[{}-{}|{}]\t", e.v, e.w, e.weight);
    std::print("\nweight\t{}\n", kmst.weight);
    std::print("\n");

    prim pmst(ewg);
    auto plist{pmst.mst};
    for (const auto &e : plist)
      std::print("[{}-{}|{}]\t", e.v, e.w, e.weight);
    std::print("\nweight\t{}\n", pmst.weight);
    assert(kmst.weight == pmst.weight);
    std::print("\n");
  }
}
