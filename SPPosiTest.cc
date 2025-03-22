#include "SPAcyclic.hh"
#include "SPBellmanFord.hh"
#include "SPDikstra.hh"

template <class SP>
void printSP(const SP &sp, int v) {
  for (int i = 0; i < v; ++i) {
    std::print("v{}\t", i);
    if (sp.hasPathTo(i)) {
      ns::deque<DirectedEdge> path{sp.pathTo(i)};
      for (const auto &e : path)
        std::print("[{}->{}|{}]\t", e.from, e.to, e.weight);
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
  for (int k = 0; k < 256; k++) {
    PRINTC("EdgeWeightedDigraph", BLUE);
    EdgeWeightedDigraph EWD(v);
    generateGraph(EWD, e);
    printGraph(EWD);
    std::print("\n");

    BellmanFord BFSP(EWD, source);
    PRINTC("BellmanFord", RED);
    printSP(BFSP, v);

    EdgeWeightedDirectedCycle EDC(EWD);
    printCycle(EDC);
    std::print("\n");

    QueueBasedBF QBBF(EWD, source);
    PRINTC("QueueBasedBF", RED);
    printSP(QBBF, v);
    std::print("\n");

    assert(std::equal(BFSP.distTo.begin(), BFSP.distTo.end(),
                      QBBF.distTo.begin(), QBBF.distTo.end()));

    DikstraSP DSP(EWD, source);
    PRINTC("Dikstra", RED);
    printSP(DSP, v);
    std::print("\n");

    assert(std::equal(DSP.distTo.begin(), DSP.distTo.end(), QBBF.distTo.begin(),
                      QBBF.distTo.end()));

    LazyDikstra LDSP(EWD, source);
    PRINTC("LazyDikstra", RED);
    printSP(LDSP, v);
    std::print("\n");

    assert(std::equal(DSP.distTo.begin(), DSP.distTo.end(), LDSP.distTo.begin(),
                      LDSP.distTo.end()));

    if (!EDC.hasCycle()) {
      AcyclicSP ASP(EWD, source);
      PRINTC("AcyclicSP", RED);
      printSP(ASP, v);
      std::print("\n");

      assert(std::equal(DSP.distTo.begin(), DSP.distTo.end(),
                        ASP.distTo.begin(), ASP.distTo.end()));

      AcyclicSPX ASPX(EWD, source);

      assert(std::equal(ASPX.distTo.begin(), ASPX.distTo.end(),
                        ASP.distTo.begin(), ASP.distTo.end()));
    }

    std::print("\n\n\n\n");
  }
}
