#pragma once
#include <algorithm>
#include <cstring>
#include <initializer_list>
#include <type_traits>

namespace ns {
template <typename ITEM>
class vector {
 public:
  vector();
  explicit vector(int);
  vector(int, const ITEM);
  vector(std::initializer_list<ITEM>);
  vector(const vector &other);
  vector(vector &&other);
  vector &operator=(const vector &rhs);
  vector &operator=(vector &&rhs);
  ~vector() { destruct(); }

  using value_type = ITEM;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = int;

  constexpr ITEM &operator[](int i) const { return seq[i]; }
  constexpr ITEM &back() const { return seq[len - 1]; }
  constexpr ITEM &front() const { return seq[0]; }
  constexpr int size() const { return len; }
  constexpr int capacity() const { return cap; }
  constexpr bool empty() const { return size() == 0; }
  void reserve(int);
  template <class... Args>
  void push_back(Args &&...);
  void push_back(ITEM &&);
  void pop_back();

  constexpr ITEM *begin() { return seq; }
  constexpr ITEM *end() { return seq + len; }
  constexpr ITEM *begin() const { return seq; }
  constexpr ITEM *end() const { return seq + len; }

 private:
  int const init_cap{4};
  int len;
  int cap;
  ITEM *seq;

  ITEM *construct(const vector &src);
  void destruct();
};

template <typename ITEM>
vector<ITEM>::vector()
    : len{0}, cap{init_cap}, seq{(ITEM *)operator new(cap * sizeof(ITEM))} {}

template <typename ITEM>
vector<ITEM>::vector(int n)
    : len{n},
      cap{n > init_cap ? n : init_cap},
      seq{(ITEM *)operator new(cap * sizeof(ITEM))} {
  // std::uninitialized_value_construct(begin(), end());
  for (int i = 0; i < n; ++i) new (seq + i) ITEM();
}

template <typename ITEM>
vector<ITEM>::vector(int n, const ITEM e)
    : len{n},
      cap{n > init_cap ? n : init_cap},
      seq{(ITEM *)operator new(cap * sizeof(ITEM))} {
  // std::uninitialized_fill(begin(), end(), e);
  for (int i = 0; i < n; ++i)
    // DO NOT USE std::move(e)
    new (seq + i) ITEM(e);
}

template <typename ITEM>
vector<ITEM>::vector(std::initializer_list<ITEM> il)
    : len(il.size()),
      cap{len > init_cap ? len : init_cap},
      seq{(ITEM *)operator new(cap * sizeof(ITEM))} {
  // std::uninitialized_move(il.begin(), il.end(), seq);
  auto i{0};
  for (auto iter{il.begin()}; iter != il.end(); ++iter) seq[i++] = *iter;
}

template <typename ITEM>
vector<ITEM>::vector(const vector &other)
    : len{other.len}, cap{other.cap}, seq{construct(other)} {}

template <typename ITEM>
vector<ITEM>::vector(vector &&other)
    : len{other.len}, cap{other.cap}, seq{other.seq} {
  other.seq = nullptr;
  other.len = 0;
}

template <typename ITEM>
vector<ITEM> &vector<ITEM>::operator=(const vector &rhs) {
  if (&rhs == this) return *this;
  destruct();
  len = rhs.len;
  cap = rhs.cap;
  seq = construct(rhs);
  return *this;
}

template <typename ITEM>
vector<ITEM> &vector<ITEM>::operator=(vector &&rhs) {
  if (&rhs == this) return *this;
  destruct();
  len = rhs.len;
  cap = rhs.cap;
  seq = rhs.seq;
  rhs.seq = nullptr;
  rhs.len = 0;
  return *this;
}

template <typename ITEM>
void vector<ITEM>::reserve(int n) {
  if (n >= len) {
    ITEM *next{(ITEM *)operator new(n * sizeof(ITEM))};
    if constexpr (std::is_fundamental_v<ITEM> || std::is_pointer_v<ITEM>)
      // seq = (ITEM *)std::realloc(seq, n * sizeof(ITEM));
      std::memcpy(next, seq, len * sizeof(ITEM));
    else {
      for (int i = 0; i < len; ++i) {
        new (next + i) ITEM(std::move(seq[i]));
        (seq + i)->~ITEM();
      }
    }
    operator delete(seq);
    seq = next;
    cap = n;
  }
}

template <typename ITEM>
void vector<ITEM>::pop_back() {
  // std::destroy_at(seq + len - 1);
  (seq + len - 1)->~ITEM();
  --len;
  if (len > 0 && len == cap >> 2) reserve(cap >> 1);
}

template <typename ITEM>
template <class... Args>
void vector<ITEM>::push_back(Args &&...args) {
  if (len == cap) reserve(cap << 1);
  // std::construct_at(seq + len, std::forward<Args>(args)...);
  new (seq + len) ITEM(std::forward<Args>(args)...);
  ++len;
}

template <typename ITEM>
void vector<ITEM>::push_back(ITEM &&e) {
  if (len == cap) reserve(cap << 1);
  new (seq + len) ITEM(std::forward<ITEM>(e));
  ++len;
}

template <typename ITEM>
ITEM *vector<ITEM>::construct(const vector &src) {
  ITEM *a{(ITEM *)operator new(cap * sizeof(ITEM))};
  // std::uninitialized_copy(src.begin(), src.end(), seq);
  for (int i = 0; i < len; ++i) new (a + i) ITEM(src[i]);
  return a;
}

template <typename ITEM>
void vector<ITEM>::destruct() {
  if constexpr (!std::is_fundamental_v<ITEM> && !std::is_pointer_v<ITEM>)
    // std::destroy(begin(), end());
    for (int i = 0; i < len; ++i) (seq + i)->~ITEM();
  operator delete(seq);
}
// stack
template <typename ITEM>
class stack : private vector<ITEM> {
 public:
  ITEM &top() { return vector<ITEM>::back(); }
  void push(const ITEM e) { vector<ITEM>::push_back(e); }
  ITEM pop() {
    ITEM x = vector<ITEM>::back();
    vector<ITEM>::pop_back();
    return x;
  }
};
}  // namespace ns
