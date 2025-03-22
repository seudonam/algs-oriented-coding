#include "SPAcyclic.hh"
#include "SPBellmanFord.hh"
#include "SPFloyd.hh"

template <class SP>
void printSP(const SP &sp, int v) {
  for (int i = 0; i < v; ++i) {
    std::print("v{}\t", i);
    if (sp.hasPathTo(i)) {
      ns::deque<DirectedEdge> path{sp.pathTo(i)};
      for (const auto &e : path)
        std::print("[{}->{}|{:+}]\t", e.from, e.to, e.weight);
    } else
      std::print("No Path");
    std::print("\n");
  }
}

#define BLUE "\033[34m"
#define RED "\033[31m"
#define PRINTC(x, color) std::print("{}{}\033[0m\n", (color), (x))

int main() {
  constexpr int v{16}, e{32}, source{4};
  for (int k = 0; k < 16; k++) {
    PRINTC("EdgeWeightedDigraph", BLUE);
    EdgeWeightedDigraph EWD(v);
    AdjMatrixEdgeWeightedDigraph AMEWD(v);
    std::mt19937 mt(std::random_device{}());
    std::uniform_int_distribution rv(0, v - 1), re(-9, 9);
    ns::deque<int> a(e), b(e);
    int i{0};
    while (i < e) {
      a[i] = rv(mt), b[i] = rv(mt);
      if (a[i] == b[i]) continue;
      int j{0};
      for (; j < i; ++j) {
        if (a[i] == a[j] && b[i] == b[j]) break;
        if (a[i] == b[j] && b[i] == a[j]) break;
      }
      if (i != j) continue;
      int wt{re(mt)};
      EWD.addEdge({a[i], b[j], wt});
      AMEWD.adj[a[i]][b[i]] = wt;
      ++i;
    }
    printGraph(EWD);

    EdgeWeightedDirectedCycle EDC(EWD);
    printCycle(EDC);
    std::print("\n");

    BellmanFord BFSP(EWD, source);
    PRINTC("BellmanFord", RED);
    if (BFSP.hasNegativeCycle())
      std::print("Has Negative Cycle\n");
    else {
      printSP(BFSP, v);
      std::print("No Negative Cycle Detected\n");
    }
    std::print("\n");

    QueueBasedBF QBBF(EWD, source);
    PRINTC("QueueBasedBF", RED);
    if (QBBF.hasNegativeCycle()) {
      std::print("Has Negative Cycle\t");
      ns::deque<DirectedEdge> nc{QBBF.negativeCycle()};
      for (const auto &e : nc)
        std::print("[{}->{}|{}]\t", e.from, e.to, e.weight);
      std::print("\n");
    } else {
      printSP(QBBF, v);
      std::print("No Negative Cycle Detected\n");
      assert(std::equal(BFSP.distTo.begin(), BFSP.distTo.end(),
                        QBBF.distTo.begin(), QBBF.distTo.end()));
    }
    std::print("\n");

    if (!EDC.hasCycle()) {
      AcyclicSP ASP(EWD, source);
      std::print("AcyclicSP\n");
      printSP(ASP, v);
      std::print("\n");

      assert(std::equal(QBBF.distTo.begin(), QBBF.distTo.end(),
                        ASP.distTo.begin(), ASP.distTo.end()));

      AcyclicSPX ASPX(EWD, source);

      assert(std::equal(ASPX.distTo.begin(), ASPX.distTo.end(),
                        ASP.distTo.begin(), ASP.distTo.end()));
    }

    PRINTC("Floyd", RED);
    Floyd FW(AMEWD);
    if (!FW.negativeCycle) {
      std::print("V\t");
      for (int i = 0; i < v; ++i) std::print("v{}\t", i);
      std::print("\n");
      for (int i = 0; i < v; ++i) {
        std::print("v{}\t", i);
        for (int j = 0; j < v; ++j) std::print("{}\t", FW.distTo[i][j]);
        std::print("\n");
      }
      assert(std::equal(QBBF.distTo.begin(), QBBF.distTo.end(),
                        FW.distTo[source].begin(), FW.distTo[source].end()));
    } else {
      std::print("Has Negative Cycle\n");
    }

    std::print("\n\n\n\n");
  }
}