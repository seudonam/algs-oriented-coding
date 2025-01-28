#include <cassert>
#include <print>
#include <string>
#include <utility>
#include <vector>

struct Trie {
  struct Node {
    bool val{false};
    Node *next[128]{};
  };
  Node *root{nullptr};
  int sz{0};
  Trie() {}
  int size() { return sz; }
  bool empty() { return size() == 0; }
  bool contains(std::string_view key) { return get(root, key, 0); }
  Node *get(Node *x, std::string_view key, int d) {
    if (x == nullptr)
      return nullptr;
    if (std::cmp_equal(d, key.length()))
      return x;
    int c = key.at(d);
    return get(x->next[c], key, d + 1);
  }

  void insert(std::string_view key) { root = insert(root, key, 0); }
  Node *insert(Node *x, std::string_view key, int d) {
    if (x == nullptr)
      x = new Node;
    if (std::cmp_equal(d, key.length())) {
      if (x->val == false)
        sz++;
      x->val = true;
      return x;
    }
    int c = key.at(d);
    x->next[c] = insert(x->next[c], key, d + 1);
    return x;
  }

  std::vector<std::string> keysWithPrefix(std::string prefix) {
    std::vector<std::string> results;
    Node *x = get(root, prefix, 0);
    collect(x, prefix, results);
    return results;
  }
  void collect(Node *x, std::string &prefix,
               std::vector<std::string> &results) {
    if (x == nullptr)
      return;
    if (x->val)
      results.push_back(prefix);
    for (int c = 0; c < 128; c++) {
      prefix.push_back(c);
      collect(x->next[c], prefix, results);
      prefix.pop_back();
    }
  }

  std::string_view longestPrefixOf(std::string_view query) {
    int length = longestPrefixOf(root, query, 0, -1);
    if (length == -1)
      return "";
    return query.substr(0, length);
  }
  int longestPrefixOf(Node *x, std::string_view query, int d, int length) {
    if (x == nullptr)
      return length;
    if (x->val)
      length = d;
    if (std::cmp_equal(d, query.length()))
      return length;
    int c = query.at(d);
    return longestPrefixOf(x->next[c], query, d + 1, length);
  }

  void remove(std::string_view key) { root = remove(root, key, 0); }
  Node *remove(Node *x, std::string_view key, int d) {
    if (x == nullptr)
      return nullptr;
    if (std::cmp_equal(d, key.length())) {
      if (x->val == true)
        sz--;
      x->val = false;
    } else {
      int c = key.at(d);
      x->next[c] = remove(x->next[c], key, d + 1);
    }

    if (x->val == true)
      return x;
    for (int c = 0; c < 128; c++)
      if (x->next[c])
        return x;
    delete x;
    return nullptr;
  }

  ~Trie() {
    if (root)
      destruct(root);
  }
  void destruct(Node *x) {
    for (int c = 0; c < 128; c++)
      if (x->next[c])
        destruct(x->next[c]);
    delete x;
  }
};

int main() {
  Trie retrieval;
  retrieval.insert("size");
  retrieval.insert("empty");
  retrieval.insert("contains");
  retrieval.insert("insert");
  retrieval.insert("remove");

  assert(retrieval.size() == 5);
  assert(retrieval.contains("size"));
  assert(retrieval.contains("empty"));
  assert(retrieval.contains("contains"));
  assert(retrieval.contains("insert"));
  assert(retrieval.contains("remove"));

  retrieval.remove("size");
  retrieval.remove("empty");
  retrieval.remove("contains");
  retrieval.remove("insert");
  retrieval.remove("remove");
  assert(retrieval.empty());

  retrieval.insert("r");
  retrieval.insert("re");
  retrieval.insert("ret");
  retrieval.insert("retr");
  retrieval.insert("retri");
  retrieval.insert("retrie");
  retrieval.insert("retriev");
  retrieval.insert("retrieva");
  retrieval.insert("retrieval");
  assert(retrieval.size() == 9);
  for (const auto &e : retrieval.keysWithPrefix(""))
    std::print("{}\n", e);
  std::print("{}\n", retrieval.longestPrefixOf("retrieval"));
}