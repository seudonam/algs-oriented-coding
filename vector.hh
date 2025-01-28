#pragma once
#include <algorithm>
#include <cstring>
#include <initializer_list>
#include <type_traits>

namespace ns {
template <typename ITEM> class vector {
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
  constexpr ITEM &back() const { return seq[sz - 1]; }
  constexpr ITEM &front() const { return seq[0]; }
  constexpr int size() const { return sz; }
  constexpr int capacity() const { return cap; }
  constexpr bool empty() const { return sz == 0; }
  void reserve(int);
  template <class... Args> void push_back(Args &&...);
  void push_back(ITEM &&);
  void pop_back();

  constexpr ITEM *begin() { return seq; }
  constexpr ITEM *end() { return seq + sz; }
  constexpr ITEM *begin() const { return seq; }
  constexpr ITEM *end() const { return seq + sz; }

private:
  int const init_cap{4};
  int sz;
  int cap;
  ITEM *seq;

  ITEM *construct(const vector &src);
  void destruct();
};

template <typename ITEM>
vector<ITEM>::vector()
    : sz{0}, cap{init_cap}, seq{(ITEM *)operator new(cap * sizeof(ITEM))} {}

template <typename ITEM>
vector<ITEM>::vector(int n)
    : sz{n}, cap{n > init_cap ? n : init_cap},
      seq{(ITEM *)operator new(cap * sizeof(ITEM))} {
  // std::uninitialized_value_construct(begin(), end());
  for (int i = 0; i < n; ++i)
    new (seq + i) ITEM();
}

template <typename ITEM>
vector<ITEM>::vector(int n, const ITEM e)
    : sz{n}, cap{n > init_cap ? n : init_cap},
      seq{(ITEM *)operator new(cap * sizeof(ITEM))} {
  // std::uninitialized_fill(begin(), end(), e);
  for (int i = 0; i < n; ++i)
    // DO NOT USE std::move(e)
    new (seq + i) ITEM(e);
}

template <typename ITEM>
vector<ITEM>::vector(std::initializer_list<ITEM> il)
    : sz(il.size()), cap{sz > init_cap ? sz : init_cap},
      seq{(ITEM *)operator new(cap * sizeof(ITEM))} {
  // std::uninitialized_move(il.begin(), il.end(), seq);
  auto i{0};
  for (auto iter{il.begin()}; iter != il.end(); ++iter)
    seq[i++] = *iter;
}

template <typename ITEM>
vector<ITEM>::vector(const vector &other)
    : sz{other.sz}, cap{other.cap}, seq{construct(other)} {}

template <typename ITEM>
vector<ITEM>::vector(vector &&other)
    : sz{other.sz}, cap{other.cap}, seq{other.seq} {
  other.seq = nullptr;
  other.sz = 0;
}

template <typename ITEM>
vector<ITEM> &vector<ITEM>::operator=(const vector &rhs) {
  if (&rhs == this)
    return *this;
  destruct();
  sz = rhs.sz;
  cap = rhs.cap;
  seq = construct(rhs);
  return *this;
}

template <typename ITEM> vector<ITEM> &vector<ITEM>::operator=(vector &&rhs) {
  if (&rhs == this)
    return *this;
  destruct();
  sz = rhs.sz;
  cap = rhs.cap;
  seq = rhs.seq;
  rhs.seq = nullptr;
  rhs.sz = 0;
  return *this;
}

template <typename ITEM> void vector<ITEM>::reserve(int n) {
  if (n >= sz) {
    ITEM *next{(ITEM *)operator new(n * sizeof(ITEM))};
    if constexpr (std::is_fundamental_v<ITEM> || std::is_pointer_v<ITEM>)
      // seq = (ITEM *)std::realloc(seq, n * sizeof(ITEM));
      std::memcpy(next, seq, sz * sizeof(ITEM));
    else {
      for (int i = 0; i < sz; ++i) {
        new (next + i) ITEM(std::move(seq[i]));
        (seq + i)->~ITEM();
      }
    }
    operator delete(seq);
    seq = next;
    cap = n;
  }
}

template <typename ITEM> void vector<ITEM>::pop_back() {
  // std::destroy_at(seq + sz - 1);
  (seq + sz - 1)->~ITEM();
  --sz;
  if (sz > 0 && sz == cap >> 2)
    reserve(cap >> 1);
}

template <typename ITEM>
template <class... Args>
void vector<ITEM>::push_back(Args &&...args) {
  if (sz == cap)
    reserve(cap << 1);
  // std::construct_at(seq + sz, std::forward<Args>(args)...);
  new (seq + sz) ITEM(std::forward<Args>(args)...);
  ++sz;
}

template <typename ITEM> void vector<ITEM>::push_back(ITEM &&e) {
  if (sz == cap)
    reserve(cap << 1);
  new (seq + sz) ITEM(std::forward<ITEM>(e));
  ++sz;
}

template <typename ITEM> ITEM *vector<ITEM>::construct(const vector &src) {
  ITEM *a{(ITEM *)operator new(cap * sizeof(ITEM))};
  // std::uninitialized_copy(src.begin(), src.end(), seq);
  for (int i = 0; i < sz; ++i)
    new (a + i) ITEM(src[i]);
  return a;
}

template <typename ITEM> void vector<ITEM>::destruct() {
  if constexpr (!std::is_fundamental_v<ITEM> && !std::is_pointer_v<ITEM>)
    // std::destroy(begin(), end());
    for (int i = 0; i < sz; ++i)
      (seq + i)->~ITEM();
  operator delete(seq);
}
// stack
template <typename ITEM> class stack : private vector<ITEM> {
public:
  ITEM &top() { return vector<ITEM>::back(); }
  void push(const ITEM e) { vector<ITEM>::push_back(e); }
  ITEM pop() {
    ITEM x = vector<ITEM>::back();
    vector<ITEM>::pop_back();
    return x;
  }
};
} // namespace ns
