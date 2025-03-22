#include <mutex>
#include <print>
#include <random>
#include <shared_mutex>
#include <string_view>
#include <vector>

template <class K, class V, class RNG = std::mt19937, int MaxLevel = 8>
struct SkipList {
  struct Node {
    std::vector<Node *> forward;
    K key;
    V value;
    Node(int lvl) : forward(lvl, nullptr) {}
    Node(int lvl, K k, V v) : forward(lvl), key{k}, value{v} {}
  };
  int level;
  Node *header;
  RNG mt;
  std::uniform_int_distribution<int> rand;
  std::shared_mutex rw{};

  SkipList()
      : level{1},
        header{new Node(MaxLevel)},
        mt(std::random_device{}()),
        rand(0, 99) {}

  ~SkipList() {
    while (header) {
      Node *next{header->forward[0]};
      delete header;
      header = next;
    }
  }

  auto search(K searchKey) -> Node * {
    std::shared_lock lk(rw);
    Node *x{header};
    for (int i = level - 1; i >= 0; i--) {
      while (x->forward[i] && x->forward[i]->key < searchKey) x = x->forward[i];
    }
    x = x->forward[0];
    if (x && x->key == searchKey)
      return x;
    else
      return nullptr;
  }

  auto insert(K searchKey, V newValue) -> void {
    std::unique_lock lk(rw);
    std::vector<Node *> update(MaxLevel);
    Node *x{header};
    for (int i = level - 1; i >= 0; i--) {
      while (x->forward[i] && x->forward[i]->key < searchKey) x = x->forward[i];
      update[i] = x;
    }
    x = x->forward[0];
    if (x && x->key == searchKey)
      x->value = newValue;
    else {
      int lvl{randomLevel()};
      if (lvl > level) {
        for (int i = level; i < lvl; i++) update[i] = header;
        level = lvl;
      }

      // lvl NOT MaxLevel
      auto y{new Node(lvl, searchKey, newValue)};
      for (int i = 0; i < lvl; i++) {
        y->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = y;
      }
    }
  }

  auto remove(K searchKey) -> void {
    std::unique_lock lk(rw);
    std::vector<Node *> update(MaxLevel);
    Node *x{header};
    for (int i = level - 1; i >= 0; i--) {
      while (x->forward[i] && x->forward[i]->key < searchKey) x = x->forward[i];
      update[i] = x;
    }
    x = x->forward[0];
    if (x && x->key == searchKey) {
      for (int i = 0; i < level; i++) {
        if (update[i]->forward[i] != x) break;
        update[i]->forward[i] = x->forward[i];
      }
      delete x;
      while (level > 1 && header->forward[level - 1] == nullptr) level--;
    }
  }

  auto randomLevel() -> int {
    int lvl{1};
    while (rand(mt) < 50 && lvl < MaxLevel) lvl++;
    return lvl;
  }

  auto printList() -> void {
    for (auto e{header->forward[0]}; e; e = e->forward[0])
      std::print("{}->{}\t", e->key, e->value);
  }
};

auto main() -> int {
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution rand(10, 99);
  SkipList<int, int> l;
  std::vector<int> a(16);

  std::print("\033[34m");
  for (auto &e : a) {
    e = rand(mt);
    l.insert(e, e);
    l.printList();
    std::print("\n");
  }

  std::print("\033[31m");
  for (const auto &e : a) {
    if (l.search(e)) l.remove(e);
    l.printList();
    std::print("\n");
  }
}
