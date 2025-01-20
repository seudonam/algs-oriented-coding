#include <print>
#include <random>
#include <vector>

template <class K, class V> struct SkipList {
  struct Node {
    std::vector<Node *> forward;
    K key;
    V value;
    Node(int lvl) : forward(lvl, nullptr) {}
    Node(int lvl, K k, V v) : forward(lvl), key{k}, value{v} {}
  };
  const int MaxLevel;
  int level;
  Node *header;
  std::mt19937 mt;
  std::uniform_int_distribution<int> rand;

  SkipList(int Max)
      : MaxLevel{Max}, level{1}, header{new Node(Max)},
        mt(std::random_device{}()), rand(0, 99) {}

  ~SkipList() {
    while (header) {
      Node *next{header->forward[0]};
      delete header;
      header = next;
    }
  }

  Node *search(K searchKey) const {
    Node *x{header};
    for (int i = level - 1; i >= 0; i--) {
      while (x->forward[i] && x->forward[i]->key < searchKey)
        x = x->forward[i];
    }
    x = x->forward[0];
    if (x && x->key == searchKey)
      return x;
    else
      return nullptr;
  }

  void insert(K searchKey, V newValue) {
    std::vector<Node *> update(MaxLevel);
    Node *x{header};
    for (int i = level - 1; i >= 0; i--) {
      while (x->forward[i] && x->forward[i]->key < searchKey)
        x = x->forward[i];
      update[i] = x;
    }
    x = x->forward[0];
    if (x && x->key == searchKey)
      x->value = newValue;
    else {
      int lvl = randomLevel();
      if (lvl > level) {
        for (int i = level; i < lvl; i++)
          update[i] = header;
        level = lvl;
      }

      x = new Node(MaxLevel, searchKey, newValue);
      for (int i = 0; i < lvl; i++) {
        x->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = x;
      }
    }
  }

  void remove(K searchKey) {
    std::vector<Node *> update(MaxLevel);
    Node *x{header};
    for (int i = level - 1; i >= 0; i--) {
      while (x->forward[i] && x->forward[i]->key < searchKey)
        x = x->forward[i];
      update[i] = x;
    }
    x = x->forward[0];
    if (x && x->key == searchKey) {
      for (int i = 0; i < level; i++) {
        if (update[i]->forward[i] != x)
          break;
        update[i]->forward[i] = x->forward[i];
      }
      delete x;
      while (level > 1 && header->forward[level - 1] == nullptr)
        level--;
    }
  }

  int randomLevel() {
    int lvl{1};
    while (rand(mt) < 50 && lvl < MaxLevel)
      lvl++;
    return lvl;
  }

  void printList() {
    for (auto e{header->forward[0]}; e; e = e->forward[0])
      std::print("({},{})\t", e->key, e->value);
  }
};

int main() {
  std::mt19937 mt(std::random_device{}());
  std::uniform_int_distribution rand(10, 99);
  SkipList<int, int> l(8);
  std::vector<int> a(16);

  for (auto &e : a) {
    e = rand(mt);
    l.insert(e, e);
    l.printList();
    std::print("\n");
  }

  for (const auto &e : a) {
    if (l.search(e))
      l.remove(e);
    l.printList();
    std::print("\n");
  }
}
