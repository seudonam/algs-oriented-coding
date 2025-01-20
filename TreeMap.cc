#include <print>
#include <random>
#include <vector>

/**
 *  based on CLRS and JDK
 *  red black tree is a binary search tree
 *  each node is marked as red || black
 *  black height of root null path is same
 *  null is black
 *  root is black
 *     black
 *       |
 *      red
 *     /   \
 *  black black
 */

template <class K, class V> struct TreeMap {
  enum mark { black, red };
  struct Node {
    K key;
    V val;
    mark color;
    Node *left, *right, *p;
    Node(K k, V v, mark c = red, Node *l = nullptr, Node *r = nullptr)
        : key{k}, val{v}, color{c}, left{l}, right{r} {}
  };
  Node *root{nullptr};

  mark color(Node *x) const {
    if (x == nullptr || x->color == black)
      return black;
    else
      return red;
  }

  // x->key > x->left->key
  // x->key <= x->right->key
  Node *search(K k) const { return search(root, k); }
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
    Node *z = new Node(key, val);
    insert(z);
    insertFixup(z);
  }
  void insert(Node *z) {
    Node *x{root};
    Node *y{nullptr};
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
  void insertFixup(Node *z) {
    while (color(z->p) == red) {
      if (z->p == z->p->p->left) {
        Node *y{z->p->p->right};
        if (color(y) == red) {
          z->p->color = black;
          y->color = black;
          z->p->p->color = red;
          z = z->p->p;
        } else {
          if (z == z->p->right) {
            z = z->p;
            leftRotate(z);
          }
          z->p->color = black;
          z->p->p->color = red;
          rightRotate(z->p->p);
        }
      } else {
        Node *y{z->p->p->left};
        if (color(y) == red) {
          z->p->color = black;
          y->color = black;
          z->p->p->color = red;
          z = z->p->p;
        } else {
          if (z == z->p->left) {
            z = z->p;
            rightRotate(z);
          }
          z->p->color = black;
          z->p->p->color = red;
          leftRotate(z->p->p);
        }
      }
    }
    root->color = black;
  }

  void remove(K k) {
    Node *x{search(root, k)};
    if (x)
      remove(x);
  }
  void remove(Node *z) {
    if (z->left && z->right) {
      Node *x = minimum(z->right);
      z->key = x->key;
      z->val = x->val;
      z = x;
    }

    Node *y = (z->left ? z->left : z->right);
    if (y) {
      y->p = z->p;
      if (z->p == nullptr)
        root = y;
      else if (z == z->p->left)
        z->p->left = y;
      else
        z->p->right = y;

      z->left = z->right = z->p = nullptr;

      if (z->color == black)
        removeFixup(y);
    } else if (z->p == nullptr)
      root = nullptr;
    else {
      if (z->color == black)
        removeFixup(z);
      if (z->p) {
        if (z == z->p->left)
          z->p->left = nullptr;
        else if (z == z->p->right)
          z->p->right = nullptr;
        z->p = nullptr;
      }
    }
    delete z;
  }

  void removeFixup(Node *x) {
    while (x != root && x->color == black) {
      if (x == x->p->left) {
        Node *w = x->p->right;

        if (color(w) == red) {
          w->color = black;
          x->p->color = red;
          leftRotate(x->p);
          w = x->p->right;
        }

        if (color(w->left) == black && color(w->right) == black) {
          w->color = red;
          x = x->p;
        } else {
          if (color(w->right) == black) {
            w->left->color = black;
            w->color = red;
            rightRotate(w);
            w = x->p->right;
          }

          w->color = color(x->p);
          x->p->color = black;
          w->right->color = black;
          leftRotate(x->p);
          x = root;
        }
      } else {
        Node *w = x->p->left;

        if (color(w) == red) {
          w->color = black;
          x->p->color = red;
          rightRotate(x->p);
          w = x->p->left;
        }

        if (color(w->right) == black && color(w->left) == black) {
          w->color = red;
          x = x->p;
        } else {
          if (color(w->left) == black) {
            w->right->color = black;
            w->color = red;
            leftRotate(w);
            w = x->p->left;
          }

          w->color = color(x->p);
          x->p->color = black;
          w->left->color = black;
          rightRotate(x->p);
          x = root;
        }
      }
    }
    x->color = black;
  }

  Node *minimum(Node *x) const {
    while (x->left)
      x = x->left;
    return x;
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

  TreeMap() {}
  ~TreeMap() { destroy(root); }
  void destroy(Node *x) {
    if (x == nullptr)
      return;
    destroy(x->left);
    destroy(x->right);
    delete x;
  }

  void preWalk() const { preWalk(root); }
  void preWalk(Node *x) const {
    if (x == nullptr)
      return;
    std::print("{} | ", x->key);
    preWalk(x->left);
    preWalk(x->right);
  }

  void inWalk() const { inWalk(root); }
  void inWalk(Node *x) const {
    if (x == nullptr)
      return;
    inWalk(x->left);
    std::print("{} | ", x->key);
    inWalk(x->right);
  }

  bool isRedBlack(K min, K max) const {
    if (color(root) != black)
      return false;
    std::vector<Node *> p_of_null;
    if (!isRedBlack(root, min, max, p_of_null))
      return false;
    int n = p_of_null.size();
    std::vector<int> black_height(n, 0);
    for (int i = 0; i < n; i++) {
      Node *x{p_of_null[i]};
      while (x) {
        if (x->color == black)
          black_height[i]++;
        x = x->p;
      }
      std::print("{} | ", black_height[i]);
    }
    for (int i = 0; i < n; i++)
      if (black_height[i] != black_height[0])
        return false;
    return true;
  }

  bool isRedBlack(Node *x, K min, K max, std::vector<Node *> &p_of_null) const {
    if (x == nullptr)
      return true;
    if (x->key < min || max <= x->key)
      return false;
    if (color(x) == red) {
      if (color(x->p) != black)
        return false;
      if (color(x->left) != black || color(x->right) != black)
        return false;
    }
    if (x->left == nullptr || x->right == nullptr)
      p_of_null.push_back(x);
    return isRedBlack(x->left, min, x->key, p_of_null) &&
           isRedBlack(x->right, x->key, max, p_of_null);
  }
};

int main() {
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution rand(100, 999);
  TreeMap<int, int> TreeMap;
  std::vector<int> a(24);

  for (auto &e : a) {
    e = rand(mt);
    std::print("\nInsert\t{}", e);
    TreeMap.insert(e, e);
    std::print("\npreWalk\t");
    TreeMap.preWalk();
    std::print("\ninWalk\t");
    TreeMap.inWalk();
    std::print("\n");
    std::print("{}", TreeMap.isRedBlack(0x80000000, 0x7fffffff));
    std::print("\n");
  }

  for (const auto &e : a) {
    std::print("\nremove\t{}", e);
    if (TreeMap.search(e))
      TreeMap.remove(e);
    std::print("\npreWalk\t");
    TreeMap.preWalk();
    std::print("\ninWalk\t");
    TreeMap.inWalk();
    std::print("\n");
    std::print("{}", TreeMap.isRedBlack(0x80000000, 0x7fffffff));
    std::print("\n");
  }
}