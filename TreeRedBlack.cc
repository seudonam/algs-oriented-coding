#include <print>
#include <random>
#include <vector>

/**
 *  based on CLRS and JDK
 *  red black tree is binary search tree
 *  each node is marked as red or black
 *  black height of root nil path is same
 *  nil is black
 *  root is black
 *     black
 *       |
 *      red
 *     /   \
 *  black black
 */

template <class K, class V>
struct RedBlackTree {
  enum mark { black, red };

  struct Node {
    K key;
    V val;
    Node *left, *right, *p;
    mark color;
    Node(K k, V v, Node *l, Node *r, mark c = red)
        : key{k}, val{v}, left{l}, right{r}, color{c} {}
  };

  Node *nil, *root;

  RedBlackTree()
      : nil{new Node(K{}, V{}, nullptr, nullptr, black)}, root{nil} {}

  ~RedBlackTree() {
    destroy(root);
    delete nil;
  }

  void destroy(Node *x) {
    if (x == nil) return;
    destroy(x->left);
    destroy(x->right);
    delete x;
  }

  // either x->key > x->left->key or x->key <= x->right->key
  Node *search(K k) const { return search(root, k); }

  Node *search(Node *x, K k) const {
    while (x != nil && k != x->key) {
      if (k < x->key)
        x = x->left;
      else
        x = x->right;
    }
    return x;
  }

  void insert(K key, V val) {
    Node *z{new Node(key, val, nil, nil)};
    insert(z);
    insertFixup(z);
  }

  void insert(Node *z) {
    Node *x{root}, *y{nil};
    while (x != nil) {
      y = x;
      if (z->key < x->key)
        x = x->left;
      else
        x = x->right;
    }
    z->p = y;
    if (y == nil)
      root = z;
    else if (z->key < y->key)
      y->left = z;
    else
      y->right = z;
  }

  void insertFixup(Node *z) {
    while (z->p->color == red) {
      if (z->p == z->p->p->left) {
        Node *y{z->p->p->right};
        if (y->color == red) {
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
        if (y->color == red) {
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
    if (x != nil) remove(x);
  }

  void remove(Node *z) {
    Node *x, *y{z};
    mark y_original_color{y->color};
    if (z->left == nil) {
      x = z->right;
      transplant(z, z->right);
    } else if (z->right == nil) {
      x = z->left;
      transplant(z, z->left);
    } else {
      y = minimum(z->right);
      y_original_color = y->color;
      x = y->right;
      if (y != z->right) {
        transplant(y, y->right);
        y->right = z->right;
        y->right->p = y;
      } else {
        x->p = y;
      }
      transplant(z, y);
      y->left = z->left;
      y->left->p = y;
      y->color = z->color;
    }
    if (y_original_color == black) removeFixup(x);
    delete z;
  }

  void removeFixup(Node *x) {
    while (x != root && x->color == black) {
      if (x == x->p->left) {
        Node *w{x->p->right};
        if (w->color == red) {
          w->color = black;
          x->p->color = red;
          leftRotate(x->p);
          w = x->p->right;
        }

        if (w->left->color == black && w->right->color == black) {
          w->color = red;
          x = x->p;
        } else {
          if (w->right->color == black) {
            w->left->color = black;
            w->color = red;
            rightRotate(w);
            w = x->p->right;
          }

          w->color = x->p->color;
          x->p->color = black;
          w->right->color = black;
          leftRotate(x->p);
          x = root;
        }
      } else {
        Node *w{x->p->left};
        if (w->color == red) {
          w->color = black;
          x->p->color = red;
          rightRotate(x->p);
          w = x->p->left;
        }

        if (w->right->color == black && w->left->color == black) {
          w->color = red;
          x = x->p;
        } else {
          if (w->left->color == black) {
            w->right->color = black;
            w->color = red;
            leftRotate(w);
            w = x->p->left;
          }

          w->color = x->p->color;
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
    while (x->left != nil) x = x->left;
    return x;
  }

  void transplant(Node *u, Node *v) {
    if (u->p == nil)
      root = v;
    else if (u == u->p->left)
      u->p->left = v;
    else
      u->p->right = v;
    v->p = u->p;
  }

  void leftRotate(Node *x) {
    Node *y{x->right};
    x->right = y->left;
    if (y->left != nil) y->left->p = x;
    y->p = x->p;
    if (x->p == nil)
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
    if (y->right != nil) y->right->p = x;
    y->p = x->p;
    if (x->p == nil)
      root = y;
    else if (x == x->p->left)
      x->p->left = y;
    else
      x->p->right = y;
    y->right = x;
    x->p = y;
  }

  void preWalk() const { preWalk(root); }

  void preWalk(Node *x) const {
    if (x == nil) return;
    std::print("{} | ", x->key);
    preWalk(x->left);
    preWalk(x->right);
  }

  void inWalk() const { inWalk(root); }

  void inWalk(Node *x) const {
    if (x == nil) return;
    inWalk(x->left);
    std::print("{} | ", x->key);
    inWalk(x->right);
  }

  bool isRedBlack(K min, K max) const {
    if (root->color != black) return false;
    std::vector<Node *> p_of_nil;
    if (!isRedBlack(root, min, max, p_of_nil)) return false;
    int n = p_of_nil.size();
    std::vector<int> black_height(n, 0);
    for (int i = 0; i < n; i++) {
      Node *x{p_of_nil[i]};
      while (x != nil) {
        if (x->color == black) black_height[i]++;
        x = x->p;
      }
      std::print("{} | ", black_height[i]);
    }
    for (int i = 0; i < n; i++)
      if (black_height[i] != black_height[0]) return false;
    return true;
  }

  bool isRedBlack(Node *x, K min, K max, std::vector<Node *> &p_of_nil) const {
    if (x == nil) return true;
    if (x->key < min || max <= x->key) return false;
    if (x->color == red) {
      if (x->p->color != black) return false;
      if (x->left->color != black || x->right->color != black) return false;
    }
    if (x->left == nil || x->right == nil) p_of_nil.push_back(x);
    return isRedBlack(x->left, min, x->key, p_of_nil) &&
           isRedBlack(x->right, x->key, max, p_of_nil);
  }
};

int main() {
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution rand(100, 999);
  RedBlackTree<int, int> RedBlackTree;
  std::vector<int> a(24);

  for (auto &e : a) {
    e = rand(mt);
    std::print("\nInsert\t{}", e);
    RedBlackTree.insert(e, e);
    std::print("\npreWalk\t");
    RedBlackTree.preWalk();
    std::print("\ninWalk\t");
    RedBlackTree.inWalk();
    std::print("\n");
    std::print("{}", RedBlackTree.isRedBlack(0x80000000, 0x7fffffff));
    std::print("\n");
  }

  for (const auto &e : a) {
    std::print("\nremove\t{}", e);
    if (RedBlackTree.search(e)) RedBlackTree.remove(e);
    std::print("\npreWalk\t");
    RedBlackTree.preWalk();
    std::print("\ninWalk\t");
    RedBlackTree.inWalk();
    std::print("\n");
    std::print("{}", RedBlackTree.isRedBlack(0x80000000, 0x7fffffff));
    std::print("\n");
  }
}