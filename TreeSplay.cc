#include <concepts>
#include <print>
#include <random>
#include <vector>

// 伸展树
template <typename K, typename V>
  requires std::totally_ordered<K>
struct SplayTree {
  struct Node {
    K key;
    V val;
    Node *p, *left, *right;
    Node(K k, V v, Node *l = nullptr, Node *r = nullptr)
        : key{k}, val{v}, left{l}, right{r} {}
  };
  Node *root{nullptr};

  SplayTree() {}
  ~SplayTree() { destroy(root); }
  void destroy(Node *x) {
    if (x == nullptr)
      return;
    destroy(x->left);
    destroy(x->right);
    delete x;
  }

  void inWalk() const { inWalk(root); }
  void inWalk(Node *x) const {
    std::vector<Node *> s;
    while (true) {
      while (x) {
        s.push_back(x);
        x = x->left;
      }
      if (s.empty())
        break;
      x = s.back();
      s.pop_back();
      std::print("{} | ", x->key);
      x = x->right;
    }
  }
  void preWalk() const { preWalk(root); }
  void preWalk(Node *x) const {
    std::vector<Node *> s;
    while (true) {
      while (x) {
        std::print("{} | ", x->key);
        s.push_back(x->right);
        x = x->left;
      }
      if (s.empty())
        break;
      x = s.back();
      s.pop_back();
    }
  }
  void postWalk() const { postWalk(root); }
  void postWalk(Node *x) const {
    std::vector<Node *> s;
    if (x)
      s.push_back(x);
    while (!s.empty()) {
      if (s.back() != x->p) {
        while (s.back()) {
          Node *y = s.back();
          if (y->left) {
            if (y->right)
              s.push_back(y->right);
            s.push_back(y->left);
          } else
            s.push_back(y->right);
        }
        s.pop_back();
      }
      x = s.back();
      s.pop_back();
      std::print("{} | ", x->key);
    }
  }

  Node *search(K k) {
    Node *x{search(root, k)};
    if (x)
      splay(x);
    return x;
  }
  Node *search(Node *x, K k) const {
    while (x && k != x->key) {
      if (k < x->key)
        x = x->left;
      else
        x = x->right;
    }
    return x;
  }

  void insert(K key, V val) {
    Node *z{new Node(key, val)};
    insert(z);
    splay(z);
  }
  void insert(Node *z) {
    Node *x{root}, *y{nullptr};
    while (x) {
      y = x;
      if (z->key < x->key)
        x = x->left;
      else
        x = x->right;
    }
    z->p = y;
    if (y == nullptr)
      root = z;
    else if (z->key < y->key)
      y->left = z;
    else
      y->right = z;
  }

  void remove(K k) {
    Node *x{search(root, k)};
    if (x)
      remove(x);
  }
  void remove(Node *z) {
    if (z->left == nullptr)
      transplant(z, z->right);
    else if (z->right == nullptr)
      transplant(z, z->left);
    else {
      Node *y = minimum(z->right);
      if (y != z->right) {
        transplant(y, y->right);
        y->right = z->right;
        y->right->p = y;
      }
      transplant(z, y);
      y->left = z->left;
      y->left->p = y;
    }
    delete z;
  }

  void splay(Node *x) {
    while (x->p && x->p->p) {
      if (x == x->p->left) {
        if (x->p == x->p->p->left) {
          rightRotate(x->p->p);
          rightRotate(x->p);
        } else {
          rightRotate(x->p);
          leftRotate(x->p);
        }
      } else {
        if (x->p == x->p->p->right) {
          leftRotate(x->p->p);
          leftRotate(x->p);
        } else {
          leftRotate(x->p);
          rightRotate(x->p);
        }
      }
    }
    if (x->p) {
      if (x == x->p->left)
        rightRotate(x->p);
      else
        leftRotate(x->p);
    }
  }

  void transplant(Node *u, Node *v) {
    if (u->p == nullptr)
      root = v;
    else if (u == u->p->left)
      u->p->left = v;
    else
      u->p->right = v;
    if (v)
      v->p = u->p;
  }

  void leftRotate(Node *x) {
    Node *y{x->right};
    x->right = y->left;
    if (y->left)
      y->left->p = x;
    y->p = x->p;
    if (x->p == nullptr)
      root = y;
    else if (x == x->p->left)
      x->p->left = y;
    else
      x->p->right = y;
    y->left = x;
    x->p = y;
  }

  void rightRotate(Node *x) {
    Node *y{x->left};
    x->left = y->right;
    if (y->right)
      y->right->p = x;
    y->p = x->p;
    if (x->p == nullptr)
      root = y;
    else if (x == x->p->left)
      x->p->left = y;
    else
      x->p->right = y;
    y->right = x;
    x->p = y;
  }

  Node *minimum(Node *x) const {
    while (x->left)
      x = x->left;
    return x;
  }

  Node *maximum(Node *x) const {
    while (x->right)
      x = x->right;
    return x;
  }

  int height() const { return height(root); }
  int height(Node *x) const {
    if (x == nullptr)
      return -1;
    return std::max(height(x->left), height(x->right)) + 1;
  }

  bool isBST(K x) const {
    int max{x};
    return isBST(root, max);
  }
  bool isBST(Node *x, K &max) const {
    if (x == nullptr)
      return true;
    if (!isBST(x->left, max))
      return false;
    if (max > x->key)
      return false;
    std::print("{} | ", x->key);
    max = x->key;
    if (!isBST(x->right, max))
      return false;
    return true;
  }
};

int main() {
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution rand(100, 999);
  SplayTree<int, int> SplayTree;
  std::vector<int> a(16);

  for (auto &e : a) {
    e = rand(mt);
    std::print("\nInsert\t{}", e);
    SplayTree.insert(e, e);
    std::print("\nheight\t{}", SplayTree.height());

    std::print("\npreWalk\t");
    SplayTree.preWalk();
    std::print("\nposWalk\t");
    SplayTree.postWalk();
    std::print("\ninWalk\t");
    SplayTree.inWalk();

    std::print("\nisBST\t");
    std::print("{}", SplayTree.isBST(0x80000000));
    std::print("\n");
  }

  for (const auto &e : a) {
    std::print("\nremove\t{}", e);
    std::print("\nheight\t{}", SplayTree.height());

    if (SplayTree.search(e))
      SplayTree.remove(e);
    std::print("\npreWalk\t");
    SplayTree.preWalk();
    std::print("\nposWalk\t");
    SplayTree.postWalk();
    std::print("\ninWalk\t");
    SplayTree.inWalk();

    std::print("\nisBST\t");
    std::print("{}", SplayTree.isBST(0x80000000));
    std::print("\n");
  }
}