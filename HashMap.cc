#include <cassert>
#include <concepts>
#include <functional>
#include <print>
#include <random>

template <class Key, class Value>
  requires std::equality_comparable<Key>
struct SequentialSearchST {
  struct Node {
    Key key;
    Value val;
    Node *next;
    Node(Key k, Value v, Node *next) : key{k}, val{v}, next{next} {}
  };
  Node *first{nullptr};
  int sz{0};
  SequentialSearchST() {}
  ~SequentialSearchST() {
    while (first) {
      Node *next{first->next};
      delete first;
      first = next;
    }
  }
  int size() { return sz; }
  bool empty() { return size() == 0; }

  bool contains(Key key) { return search(key); }
  Node *search(Key key) {
    for (Node *x = first; x; x = x->next) {
      if (key == x->key) return x;
    }
    return nullptr;
  }

  void insert(Key key, Value val) {
    for (Node *x = first; x; x = x->next)
      if (key == x->key) {
        x->val = val;
        return;
      }
    first = new Node(key, val, first);
    ++sz;
  }

  void remove(Key key) { first = remove(first, key); }
  Node *remove(Node *x, Key key) {
    if (x == nullptr) return nullptr;
    if (key == x->key) {
      --sz;
      Node *y = x->next;
      delete x;
      return y;
    }
    x->next = remove(x->next, key);
    return x;
  }

  struct iterator {
    Node *ptr;
    iterator(Node *ptr) : ptr(ptr) {}
    Node &operator*() const { return *ptr; }
    bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
    bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
    iterator &operator++() {
      ptr = ptr->next;
      return *this;
    }
    iterator operator++(int) {
      iterator x{*this};
      ptr = ptr->next;
      return x;
    }
  };

  iterator begin() { return iterator(first); }
  iterator end() { return iterator(nullptr); }
};

template <typename T>
concept is_hashable = requires(T a) {
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template <class Key, class Value>
  requires is_hashable<Key>
struct HashMap {
  int INIT_CAPACITY{4}, N, M;
  std::hash<Key> hashCode;
  SequentialSearchST<Key, Value> *st;

  HashMap()
      : N{0},
        M{INIT_CAPACITY},
        hashCode{},
        st{new SequentialSearchST<Key, Value>[M]} {}

  HashMap(int M)
      : N{0}, M{M}, hashCode{}, st{new SequentialSearchST<Key, Value>[M]} {}

  ~HashMap() { delete[] st; }

  int hash(Key key) const { return (hashCode(key) & 0x7fffffffffffffff) % M; }
  bool contains(Key key) const { return search(key); }
  auto search(Key key) const { return st[hash(key)].search(key); }

  void insert(Key key, Value val) {
    if (!contains(key)) N++;
    st[hash(key)].insert(key, val);
    if (loadFactor() >= 10) resize(2 * M);
  }

  void remove(Key key) {
    if (contains(key)) N--;
    st[hash(key)].remove(key);
    if (INIT_CAPACITY < M && loadFactor() <= 2) resize(M / 2);
  }

  void resize(int x) {
    auto next{new SequentialSearchST<Key, Value>[x]};
    for (int i = 0; i < M; i++) {
      for (const auto &e : st[i]) next[hash(e.key)].insert(e.key, e.val);
    }
    delete[] st;
    st = next;
    M = x;
  }

  auto loadFactor() { return N / M; }
  int size() const { return N; }
  bool empty() const { return size() == 0; }
};

int main() {
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution rand(100, 999);
  HashMap<int, int> HashMap;
  std::vector<int> a(16);

  for (auto &e : a) {
    e = rand(mt);
    std::print("Insert\t{}", e);
    HashMap.insert(e, e);
    std::print("\n");
  }

  for (const auto &e : a) {
    std::print("remove\t{}", e);
    if (HashMap.search(e)) HashMap.remove(e);
    std::print("\n");
  }
}