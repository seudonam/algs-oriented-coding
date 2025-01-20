// union by rank with path compression
struct UF {
  int *parent;
  int *rank;
  UF(int n) : parent{new int[n]}, rank{new int[n]{}} {
    for (int i = 0; i < n; i++)
      parent[i] = i;
  }
  ~UF() {
    delete[] parent;
    delete[] rank;
  }
  int find(int p) {
    if (p != parent[p])
      parent[p] = find(parent[p]);
    return parent[p];
  }
  void merge(int p, int q) { join(find(p), find(q)); }
  void join(int x, int y) {
    if (rank[x] > rank[y])
      parent[y] = x;
    else {
      parent[x] = y;
      if (rank[x] == rank[y])
        rank[y]++;
    }
  }
};

struct WeightedQuickUnionUF {
  int *parent;
  int *size;
  int count;
  WeightedQuickUnionUF(int n)
      : parent{new int[n]}, size{new int[n]{}}, count{n} {
    for (int i = 0; i < n; i++) {
      parent[i] = i;
      size[i] = 1;
    }
  }
  ~WeightedQuickUnionUF() {
    delete[] parent;
    delete[] size;
  }
  int find(int p) {
    while (p != parent[p])
      p = parent[p];
    return p;
  }
  void merge(int p, int q) {
    int rootP{find(p)}, rootQ{find(q)};
    if (rootP == rootQ)
      return;
    if (size[rootP] < size[rootQ]) {
      parent[rootP] = rootQ;
      size[rootQ] += size[rootP];
    } else {
      parent[rootQ] = rootP;
      size[rootP] += size[rootQ];
    }
    count--;
  }
  bool connected(int p, int q) { return find(p) == find(q); }
};

struct QuickUnionUF {
  int *parent;
  int count;
  QuickUnionUF(int n) : parent{new int[n]}, count{n} {
    for (int i = 0; i < n; i++)
      parent[i] = i;
  }
  ~QuickUnionUF() { delete[] parent; }
  int find(int p) {
    while (p != parent[p])
      p = parent[p];
    return p;
  }
  void merge(int p, int q) {
    int rootP{find(p)}, rootQ{find(q)};
    if (rootP == rootQ)
      return;
    parent[rootP] = rootQ;
    count--;
  }
  bool connected(int p, int q) { return find(p) == find(q); }
};

struct QuickFindUF {
  int *parent;
  int sz;
  int count;
  QuickFindUF(int n) : parent{new int[n]}, sz{n}, count{n} {
    for (int i = 0; i < n; i++)
      parent[i] = i;
  }
  ~QuickFindUF() { delete[] parent; }
  int find(int p) { return parent[p]; }
  void merge(int p, int q) {
    int rootP{parent[p]}, rootQ{parent[q]};
    if (rootP == rootQ)
      return;
    for (int i = 0; i < sz; i++)
      if (parent[i] == rootP)
        parent[i] = rootQ;
    count--;
  }
  bool connected(int p, int q) { return find(p) == find(q); }
};