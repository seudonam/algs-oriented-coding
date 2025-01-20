#include <concepts>
#include <print>
#include <random>
#include <vector>

template <typename Key, typename Value>
  requires std::totally_ordered<Key>
struct AVLTree {
  struct Node {
    Key key;
    Value val;
    int height;
    Node *left{nullptr}, *right{nullptr};
    Node(Key key, Value val, int h = 0) : key{key}, val{val}, height{h} {}
  };
  Node *root{nullptr};

  AVLTree() {}
  ~AVLTree() { destroy(root); }
  void destroy(Node *x) {
    if (x == nullptr)
      return;
    destroy(x->left);
    destroy(x->right);
    delete x;
  }

  int height() const { return height(root); }
  int height(Node *x) const {
    if (x == nullptr)
      return -1;
    return x->height;
  }

  int balanceFactor(Node *x) const {
    return height(x->left) - height(x->right);
  }

  Node *search(Key key) const { return search(root, key); }
  Node *search(Node *x, Key key) const {
    if (x == nullptr)
      return nullptr;
    if (key < x->key)
      return search(x->left, key);
    else if (key > x->key)
      return search(x->right, key);
    else
      return x;
  }

  void insert(Key key, Value val) { root = insert(root, key, val); }
  Node *insert(Node *x, Key key, Value val) {
    if (x == nullptr)
      return new Node(key, val);
    if (key < x->key)
      x->left = insert(x->left, key, val);
    else if (key > x->key)
      x->right = insert(x->right, key, val);
    else
      return x;
    x->height = 1 + std::max(height(x->left), height(x->right));
    return balance(x);
  }

  void remove(Key key) {
    if (!search(root, key))
      return;
    root = remove(root, key);
  }
  Node *remove(Node *x, Key key) {
    if (key < x->key)
      x->left = remove(x->left, key);
    else if (key > x->key)
      x->right = remove(x->right, key);
    else {
      if (x->left == nullptr) {
        Node *y{x->right};
        delete x;
        return y;
      } else if (x->right == nullptr) {
        Node *y{x->left};
        delete x;
        return y;
      } else {
        Node *y{x};
        x = min(y->right);
        x->right = removeMin(y->right);
        x->left = y->left;
        delete y;
      }
    }
    x->height = 1 + std::max(height(x->left), height(x->right));
    return balance(x);
  }

  Node *balance(Node *x) {
    if (balanceFactor(x) < -1) {
      if (balanceFactor(x->right) > 0)
        x->right = rotateRight(x->right);
      x = rotateLeft(x);
    } else if (1 < balanceFactor(x)) {
      if (balanceFactor(x->left) < 0)
        x->left = rotateLeft(x->left);
      x = rotateRight(x);
    }
    return x;
  }

  Node *rotateLeft(Node *x) {
    Node *y{x->right};
    x->right = y->left;
    y->left = x;
    x->height = 1 + std::max(height(x->left), height(x->right));
    y->height = 1 + std::max(height(y->left), height(y->right));
    return y;
  }

  Node *rotateRight(Node *x) {
    Node *y{x->left};
    x->left = y->right;
    y->right = x;
    x->height = 1 + std::max(height(x->left), height(x->right));
    y->height = 1 + std::max(height(y->left), height(y->right));
    return y;
  }

  Node *min(Node *x) const {
    if (x->left == nullptr) {
      return x;
    }
    return min(x->left);
  }

  Node *removeMin(Node *x) {
    if (x->left == nullptr) {
      return x->right;
    }
    x->left = removeMin(x->left);
    x->height = 1 + std::max(height(x->left), height(x->right));
    return balance(x);
  }

  void inWalk() const { inWalk(root); }
  void inWalk(Node *x) const {
    if (x == nullptr)
      return;
    inWalk(x->left);
    std::print("{} | ", x->val);
    inWalk(x->right);
  }

  void preWalk() const { preWalk(root); }
  void preWalk(Node *x) const {
    if (x == nullptr)
      return;
    std::print("{} | ", x->val);
    preWalk(x->left);
    preWalk(x->right);
  }

  bool isAVL(Key min, Key max) const { return isAVL(root, min, max); }
  bool isAVL(Node *x, Key min, Key max) const {
    if (x == nullptr)
      return true;
    if (x->key <= min || max <= x->key)
      return false;
    if (balanceFactor(x) < -1 || 1 < balanceFactor(x))
      return false;
    return isAVL(x->left, min, x->key) && isAVL(x->right, x->key, max);
  }
};

int main() {
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution rand(100, 999);
  AVLTree<int, int> avl;
  std::vector<int> a(8);

  for (auto &e : a) {
    e = rand(mt);
    std::print("\nInsert\t{}", e);
    avl.insert(e, e);
    std::print("\ninWalk\t");
    avl.inWalk();
    std::print("\npreWalk\t");
    avl.preWalk();
    std::print("\n");
    std::print("{}", avl.isAVL(0x80000000, 0x7fffffff));
    std::print("\n");
  }

  for (const auto &e : a) {
    std::print("\nremove\t{}", e);
    if (avl.search(e))
      avl.remove(e);
    std::print("\ninWalk\t");
    avl.inWalk();
    std::print("\npreWalk\t");
    avl.preWalk();
    std::print("\n");
    std::print("{}", avl.isAVL(0x80000000, 0x7fffffff));
    std::print("\n");
  }
}
