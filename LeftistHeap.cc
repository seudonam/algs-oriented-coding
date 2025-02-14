#include <cassert>
#include <concepts>
#include <functional>
#include <print>
#include <random>
#include <utility>

// leftist heap is a binary tree
// leftist heap is also a heap

template <class T, class Comparator>
  requires std::totally_ordered<T>
struct LeftistHeap {
  struct Node {
    // parent->e >= left->e
    // parent->e >= right->e
    T e;
    Node *left{nullptr}, *right{nullptr};
    // npl(leaf) = 1
    // npl(left) >= npl(right)
    int npl{1};
    Node(T e) : e{e} {}
  };

  Comparator comparator;
  Node *root{nullptr};

  bool empty() const { return root == nullptr; }

  void insert(T x) { root = merge(root, new Node(x)); }

  T delMax() {
    T e = root->e;
    Node *l{root->left}, *r{root->right};
    delete root;
    root = merge(l, r);
    return e;
  }

  void merge(LeftistHeap &rhs) {
    if (this == &rhs)
      return;
    root = merge(root, rhs.root);
    rhs.root = nullptr;
  }

  Node *merge(Node *a, Node *b) {
    if (a == nullptr)
      return b;
    if (b == nullptr)
      return a;
    if (comparator(a->e, b->e))
      std::swap(a, b);
    // left is null => right is null
    if (a->left == nullptr)
      a->left = b;
    else {
      a->right = merge(a->right, b);
      // skew heap always swap l and r
      if (a->left->npl < a->right->npl)
        std::swap(a->left, a->right);
      a->npl = a->right->npl + 1;
    }
    return a;
  }

  LeftistHeap(auto comparator) : comparator{comparator} {}

  ~LeftistHeap() { destroy(root); }
  void destroy(Node *x) {
    if (x == nullptr)
      return;
    destroy(x->left);
    destroy(x->right);
    delete x;
  }

  bool isLeftistHeap() { return isLeftistHeap(root); }
  bool isLeftistHeap(Node *x) {
    if (x == nullptr)
      return true;
    if (x->left && comparator(x->e, x->left->e))
      return false;
    if (x->right && comparator(x->e, x->right->e))
      return false;
    if (nullPathLength(x->left) < nullPathLength(x->right))
      return false;
    return isLeftistHeap(x->left) && isLeftistHeap(x->right);
  }

  int nullPathLength(Node *x) {
    if (x == nullptr)
      return 0;
    return x->npl;
  }
};

int main() {
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution rand(10, 99);
  LeftistHeap<int, std::function<bool(int, int)>> maxpq(
      [](auto &&v, auto &&w) { return v > w; }),
      maxqp([](auto &&v, auto &&w) { return v > w; });

  std::print("vector\t");
  for (int i = 0; i < 8; i++) {
    int e = rand(mt);
    maxpq.insert(e);
    assert(maxpq.isLeftistHeap());
    maxqp.insert(e);
    assert(maxqp.isLeftistHeap());
    std::print("{}\t", e);
  }
  std::print("\n");

  maxpq.merge(maxqp);
  assert(maxpq.isLeftistHeap());

  std::print("max\t");
  while (!maxpq.empty()) {
    std::print("{}\t", maxpq.delMax());
    assert(maxpq.isLeftistHeap());
  }
  std::print("\n");
}