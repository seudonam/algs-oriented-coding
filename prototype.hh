#pragma once
#include <cstring>
#include <initializer_list>
#include <memory>
#include <type_traits>

namespace ns {
// vector
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
  ~vector() { delete[] seq; }

  ITEM &operator[](int i) const { return seq[i]; }
  ITEM &back() const { return seq[sz - 1]; }
  int size() const { return sz; }
  int capacity() const { return cap; }
  bool empty() const { return sz == 0; }
  void reserve(int);
  void push_back(const ITEM);
  void pop_back();

  ITEM *begin() { return seq; }
  ITEM *end() { return seq + sz; }
  const ITEM *begin() const { return seq; }
  const ITEM *end() const { return seq + sz; }

private:
  int const init_cap{4};
  int sz;
  int cap;
  ITEM *seq;
};

template <typename ITEM>
vector<ITEM>::vector() : sz{0}, cap{init_cap}, seq{new ITEM[cap]} {}

template <typename ITEM>
vector<ITEM>::vector(int n)
    : sz{n}, cap{n > init_cap ? n : init_cap}, seq{new ITEM[cap]} {}

template <typename ITEM>
vector<ITEM>::vector(int n, const ITEM item)
    : sz{n}, cap{n > init_cap ? n : init_cap}, seq{new ITEM[cap]} {
  for (int i = 0; i < n; ++i)
    seq[i] = item;
}

template <typename ITEM>
vector<ITEM>::vector(std::initializer_list<ITEM> il)
    : sz(il.size()), cap{sz > init_cap ? sz : init_cap}, seq{new ITEM[cap]} {
  int i{0};
  for (auto iter = il.begin(); iter != il.end(); ++iter)
    seq[i++] = *iter;
}

template <typename ITEM>
vector<ITEM>::vector(const vector &other)
    : sz{other.sz}, cap{other.cap}, seq{new ITEM[cap]} {
  for (int i = 0; i < sz; ++i)
    seq[i] = other[i];
}

template <typename ITEM>
vector<ITEM>::vector(vector &&other)
    : sz{other.sz}, cap{other.cap}, seq{other.seq} {
  other.seq = nullptr;
}

template <typename ITEM>
vector<ITEM> &vector<ITEM>::operator=(const vector &rhs) {
  if (&rhs == this)
    return *this;
  sz = rhs.sz;
  cap = rhs.cap;
  delete[] seq;
  seq = new ITEM[rhs.cap];
  for (int i = 0; i < sz; ++i)
    seq[i] = rhs[i];
  return *this;
}

template <typename ITEM> vector<ITEM> &vector<ITEM>::operator=(vector &&rhs) {
  if (&rhs == this)
    return *this;
  sz = rhs.sz;
  cap = rhs.cap;
  delete[] seq;
  seq = rhs.seq;
  rhs.seq = nullptr;
  return *this;
}

template <typename ITEM> void vector<ITEM>::reserve(int n) {
  if (n >= sz) {
    ITEM *next{new ITEM[n]};
    for (int i = 0; i < sz; ++i)
      next[i] = (ITEM &&)seq[i];
    delete[] seq;
    seq = next;
    cap = n;
  }
}

template <typename ITEM> void vector<ITEM>::pop_back() {
  --sz;
  if (sz > 0 && sz == cap >> 2)
    reserve(cap >> 1);
}

template <typename ITEM> void vector<ITEM>::push_back(const ITEM item) {
  if (sz == cap)
    reserve(cap << 1);
  seq[sz++] = item;
}

// deque
template <typename ITEM> class deque {
public:
  deque();
  explicit deque(int);
  deque(int, const ITEM);
  deque(const deque &other);
  deque(deque &&other);
  deque &operator=(const deque &rhs);
  deque &operator=(deque &&rhs);
  ~deque() { delete[] seq; }

  ITEM &operator[](int i) const { return seq[(head + i) % cap]; }
  ITEM &back() const { return seq[(head + sz - 1) % cap]; }
  ITEM &front() const { return seq[head]; }
  int size() const { return sz; }
  int capacity() const { return cap; }
  bool empty() const { return sz == 0; }

  void reserve(int);
  void push_back(const ITEM);
  void pop_back();
  void push_front(const ITEM);
  void pop_front();
  void clear();

  class iterator {
    friend class deque;

  public:
    ITEM &operator*() const { return deck[idx % cap]; }
    bool operator==(const iterator &rhs) const { return idx == rhs.idx; }
    bool operator!=(const iterator &rhs) const { return idx != rhs.idx; }
    iterator &operator++();
    iterator operator++(int);

  private:
    iterator(ITEM *deck, int cap, int idx) : deck{deck}, cap{cap}, idx(idx) {}
    ITEM *deck;
    int cap;
    int idx;
  };
  struct const_iterator {
    ITEM &operator*() const { return deck[idx % cap]; }
    bool operator==(const const_iterator &rhs) const { return idx == rhs.idx; }
    bool operator!=(const const_iterator &rhs) const { return idx != rhs.idx; }
    const_iterator &operator++();
    const_iterator operator++(int);

    const_iterator(ITEM *deck, int cap, int idx)
        : deck{deck}, cap{cap}, idx(idx) {}
    ITEM *deck;
    int cap;
    int idx;
  };
  iterator begin() { return iterator(seq, cap, head); }
  iterator end() { return iterator(seq, cap, head + sz); }
  const_iterator begin() const { return const_iterator(seq, cap, head); }
  const_iterator end() const { return const_iterator(seq, cap, head + sz); }

private:
  int const init_cap{4};
  int sz;
  int cap;
  int head;
  ITEM *seq;
};

template <typename ITEM>
deque<ITEM>::deque() : sz{0}, cap{init_cap}, head{0}, seq{new ITEM[cap]} {}

template <typename ITEM>
deque<ITEM>::deque(int n)
    : sz{n}, cap{n > init_cap ? n : init_cap}, head{0}, seq{new ITEM[cap]} {}

template <typename ITEM>
deque<ITEM>::deque(int n, const ITEM item)
    : sz{n}, cap{n > init_cap ? n : init_cap}, head{0}, seq{new ITEM[cap]} {
  for (int i = 0; i < n; ++i)
    seq[i] = item;
}

template <typename ITEM>
deque<ITEM>::deque(const deque &other)
    : sz{other.sz}, cap{other.cap}, head{0}, seq{new ITEM[cap]} {
  for (int i = 0; i < sz; ++i) {
    seq[i] = other[i];
  }
}

template <typename ITEM>
deque<ITEM>::deque(deque &&other)
    : sz{other.sz}, cap{other.cap}, head{other.head}, seq{other.seq} {
  other.seq = nullptr;
}

template <typename ITEM> deque<ITEM> &deque<ITEM>::operator=(const deque &rhs) {
  if (&rhs == this)
    return *this;
  sz = rhs.sz;
  cap = rhs.cap;
  head = 0;
  delete[] seq;
  seq = new ITEM[rhs.cap];
  for (int i = 0; i < sz; ++i)
    seq[i] = rhs[i];
  return *this;
}

template <typename ITEM> deque<ITEM> &deque<ITEM>::operator=(deque &&rhs) {
  if (&rhs == this)
    return *this;
  sz = rhs.sz;
  cap = rhs.cap;
  head = rhs.head;
  delete[] seq;
  seq = rhs.seq;
  rhs.seq = nullptr;
  return *this;
}

template <typename ITEM> void deque<ITEM>::reserve(int n) {
  if (n >= sz) {
    ITEM *next{new ITEM[n]};
    for (int i = 0; i < sz; ++i)
      next[i] = (ITEM &&)seq[(head + i) % cap];
    delete[] seq;
    seq = next;
    cap = n;
    head = 0;
  }
}

template <typename ITEM> void deque<ITEM>::push_back(const ITEM item) {
  if (sz == cap)
    reserve(cap << 1);
  ++sz;
  seq[(head + sz - 1) % cap] = item;
}

template <typename ITEM> void deque<ITEM>::pop_back() {
  --sz;
  if (sz > 0 && sz == cap >> 2)
    reserve(cap >> 1);
}

template <typename ITEM> void deque<ITEM>::push_front(const ITEM item) {
  if (sz == cap)
    reserve(cap << 1);
  ++sz;
  if (sz > 1) {
    --head;
    if (head == -1)
      head = cap - 1;
  }
  seq[head] = item;
}

template <typename ITEM> void deque<ITEM>::pop_front() {
  --sz;
  if (sz > 0) {
    ++head;
    if (head == cap)
      head = 0;
    if (sz == cap >> 2)
      reserve(cap >> 1);
  }
}

template <typename ITEM> void deque<ITEM>::clear() {
  delete[] seq;
  sz = 0;
  cap = init_cap;
  head = 0;
  seq = new ITEM[cap];
}

template <typename ITEM>
typename deque<ITEM>::iterator &deque<ITEM>::iterator::operator++() {
  ++idx;
  return *this;
}

template <typename ITEM>
typename deque<ITEM>::iterator deque<ITEM>::iterator::operator++(int) {
  iterator x{*this};
  ++idx;
  return x;
}

template <typename ITEM>
typename deque<ITEM>::const_iterator &
deque<ITEM>::const_iterator::operator++() {
  ++idx;
  return *this;
}

template <typename ITEM>
typename deque<ITEM>::const_iterator
deque<ITEM>::const_iterator::operator++(int) {
  const_iterator x{*this};
  ++idx;
  return x;
}
} // namespace ns

namespace core {
// vector
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

  constexpr ITEM &operator[](int i) const { return seq[i]; }
  constexpr ITEM &back() const { return seq[sz - 1]; }
  constexpr int size() const { return sz; }
  constexpr int capacity() const { return cap; }
  constexpr bool empty() const { return sz == 0; }
  void reserve(int);
  template <class... Args> void push_back(Args &&...args);
  void pop_back();

  ITEM *begin() { return seq; }
  ITEM *end() { return seq + sz; }
  const ITEM *begin() const { return seq; }
  const ITEM *end() const { return seq + sz; }
  const ITEM *cbegin() const { return seq; }
  const ITEM *cend() const { return seq + sz; }

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
  std::uninitialized_value_construct(begin(), end());
}

template <typename ITEM>
vector<ITEM>::vector(int n, const ITEM item)
    : sz{n}, cap{n > init_cap ? n : init_cap},
      seq{(ITEM *)operator new(cap * sizeof(ITEM))} {
  std::uninitialized_fill(begin(), end(), item);
}

template <typename ITEM>
vector<ITEM>::vector(std::initializer_list<ITEM> il)
    : sz(il.size()), cap{sz > init_cap ? sz : init_cap},
      seq{(ITEM *)operator new(cap * sizeof(ITEM))} {
  std::uninitialized_move(il.begin(), il.end(), seq);
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
    static_assert(std::is_move_constructible_v<ITEM>);
    ITEM *next{(ITEM *)operator new(n * sizeof(ITEM))};
    if constexpr (std::is_fundamental_v<ITEM> || std::is_pointer_v<ITEM>)
      // seq = (ITEM *)std::realloc(seq, n * sizeof(ITEM));
      std::memcpy(next, seq, sz * sizeof(ITEM));
    else {
      std::uninitialized_move(begin(), end(), next);
      std::destroy(begin(), end());
    }
    operator delete(seq);
    seq = next;
    cap = n;
  }
}

template <typename ITEM> void vector<ITEM>::pop_back() {
  std::destroy_at(seq + sz - 1);
  --sz;
  if (sz > 0 && sz == cap >> 2)
    reserve(cap >> 1);
}

template <typename ITEM>
template <class... Args>
void vector<ITEM>::push_back(Args &&...args) {
  if (sz == cap)
    reserve(cap << 1);
#if __cplusplus >= 202002L
  std::construct_at(seq + sz, std::forward<Args>(args)...);
#else
  new (seq + sz) ITEM(std::forward<Args>(args)...);
#endif
  ++sz;
}

template <typename ITEM> ITEM *vector<ITEM>::construct(const vector &src) {
  ITEM *a{(ITEM *)operator new(cap * sizeof(ITEM))};
  std::uninitialized_copy(src.cbegin(), src.cend(), a);
  return a;
}

template <typename ITEM> void vector<ITEM>::destruct() {
  if constexpr (!std::is_fundamental_v<ITEM> && !std::is_pointer_v<ITEM>)
    std::destroy(begin(), end());
  operator delete(seq);
}

// deque
template <typename ITEM> class deque {
public:
  deque();
  explicit deque(int);
  deque(int, const ITEM);
  deque(const deque &other);
  deque(deque &&other);
  deque &operator=(const deque &rhs);
  deque &operator=(deque &&rhs);
  ~deque() { destruct(); }

  constexpr ITEM &operator[](int i) const { return seq[(head + i) % cap]; }
  constexpr ITEM &back() const { return seq[tail]; }
  constexpr ITEM &front() const { return seq[head]; }
  constexpr int size() const { return sz; }
  constexpr int capacity() const { return cap; }
  constexpr bool empty() const { return sz == 0; }

  void reserve(int);
  template <class... Args> void push_back(Args &&...args);
  void pop_back();
  template <class... Args> void push_front(Args &&...args);
  void pop_front();
  void clear();

  struct iterator {
    using difference_type = std::ptrdiff_t;
    using value_type = ITEM;
    using pointer = ITEM *;
    using reference = ITEM &;
    using iterator_category = std::forward_iterator_tag;

    constexpr ITEM &operator*() const { return deck[idx]; }
    bool operator==(const iterator &rhs) const { return idx == rhs.idx; }
    iterator &operator++();
    iterator operator++(int);

    iterator(deque &deck, int idx) : deck{deck}, idx{idx} {}
    deque &deck;
    int idx;
  };

  struct const_iterator {
    using difference_type = std::ptrdiff_t;
    using value_type = ITEM;
    using pointer = const ITEM *;
    using reference = const ITEM &;
    using iterator_category = std::forward_iterator_tag;

    constexpr ITEM &operator*() const { return deck[idx]; }
    bool operator==(const const_iterator &rhs) const { return idx == rhs.idx; }
    const_iterator &operator++();
    const_iterator operator++(int);

    const_iterator(const deque &deck, int idx) : deck{deck}, idx{idx} {}
    const deque &deck;
    int idx;
  };

  iterator begin() { return iterator(*this, 0); }
  iterator end() { return iterator(*this, sz); }
  const_iterator begin() const { return const_iterator(*this, 0); }
  const_iterator end() const { return const_iterator(*this, sz); }
  const_iterator cbegin() const { return const_iterator(*this, 0); }
  const_iterator cend() const { return const_iterator(*this, sz); }

private:
  int const init_cap{4};
  int sz;
  int cap;
  int head;
  int tail;
  ITEM *seq;

  ITEM *construct(const deque &src);
  void destruct();
};

template <typename ITEM>
deque<ITEM>::deque()
    : sz{0}, cap{init_cap}, head{0}, tail{0},
      seq{(ITEM *)operator new(cap * sizeof(ITEM))} {}

template <typename ITEM>
deque<ITEM>::deque(int n)
    : sz{n}, cap{n > init_cap ? n : init_cap}, head{0}, tail{sz - 1},
      seq{(ITEM *)operator new(cap * sizeof(ITEM))} {
  std::uninitialized_value_construct(begin(), end());
}

template <typename ITEM>
deque<ITEM>::deque(int n, const ITEM item)
    : sz{n}, cap{n > init_cap ? n : init_cap}, head{0}, tail{sz - 1},
      seq{(ITEM *)operator new(cap * sizeof(ITEM))} {
  std::uninitialized_fill(begin(), end(), item);
}

template <typename ITEM>
deque<ITEM>::deque(const deque &other)
    : sz{other.sz}, cap{other.cap}, head{0}, tail{sz - 1},
      seq{construct(other)} {}

template <typename ITEM>
deque<ITEM>::deque(deque &&other)
    : sz{other.sz}, cap{other.cap}, head{other.head}, tail{other.tail},
      seq{other.seq} {
  other.seq = nullptr;
  other.sz = 0;
}

template <typename ITEM> deque<ITEM> &deque<ITEM>::operator=(const deque &rhs) {
  if (&rhs == this)
    return *this;
  destruct();
  sz = rhs.sz;
  cap = rhs.cap;
  head = 0;
  tail = sz - 1;
  seq = construct(rhs);
  return *this;
}

template <typename ITEM> deque<ITEM> &deque<ITEM>::operator=(deque &&rhs) {
  if (&rhs == this)
    return *this;
  destruct();
  sz = rhs.sz;
  cap = rhs.cap;
  head = rhs.head;
  tail = rhs.tail;
  seq = rhs.seq;
  rhs.seq = nullptr;
  rhs.sz = 0;
  return *this;
}

template <typename ITEM> void deque<ITEM>::reserve(int n) {
  if (n >= sz) {
    static_assert(std::is_move_constructible_v<ITEM>);
    ITEM *next{(ITEM *)operator new(n * sizeof(ITEM))};
    for (int i = 0; i < sz; ++i) {
#if __cplusplus >= 202002L
      std::construct_at(next + i, std::move(seq[(head + i) % cap]));
      std::destroy_at(seq + (head + i) % cap);
#else
      new (next + i) ITEM(std::move(seq[(head + i) % cap]));
      (seq + (head + i) % cap)->~ITEM();
#endif
    }
    operator delete(seq);
    seq = next;
    cap = n;
    head = 0;
    tail = sz - 1;
  }
}
// 对sz等于0和sz大于0两种情况分别处理
template <typename ITEM>
template <class... Args>
void deque<ITEM>::push_back(Args &&...args) {
  if (sz == cap)
    reserve(cap << 1);
  ++sz;
  if (sz > 1)
    if (++tail == cap)
      tail = 0;
#if __cplusplus >= 202002L
  std::construct_at(seq + tail, std::forward<Args>(args)...);
#else
  new (seq + tail) ITEM(std::forward<Args>(args)...);
#endif
}
// 对sz等于1和sz大于1两种情况分别处理
template <typename ITEM> void deque<ITEM>::pop_back() {
  std::destroy_at(seq + tail);
  --sz;
  if (sz > 0) {
    if (--tail == -1)
      tail = cap - 1;
    if (sz == cap >> 2)
      reserve(cap >> 1);
  }
}
// 对sz等于0和sz大于0两种情况分别处理
template <typename ITEM>
template <class... Args>
void deque<ITEM>::push_front(Args &&...args) {
  if (sz == cap)
    reserve(cap << 1);
  ++sz;
  if (sz > 1)
    if (--head == -1)
      head = cap - 1;
#if __cplusplus >= 202002L
  std::construct_at(seq + head, std::forward<Args>(args)...);
#else
  new (seq + head) ITEM(std::forward<Args>(args)...);
#endif
}
// 对sz等于1和sz大于1两种情况分别处理
template <typename ITEM> void deque<ITEM>::pop_front() {
  std::destroy_at(seq + head);
  --sz;
  if (sz > 0) {
    if (++head == cap)
      head = 0;
    if (sz == cap >> 2)
      reserve(cap >> 1);
  }
}

template <typename ITEM> void deque<ITEM>::clear() {
  destruct();
  sz = 0;
  cap = init_cap;
  head = tail = 0;
  seq = (ITEM *)operator new(cap * sizeof(ITEM));
}

template <typename ITEM>
typename deque<ITEM>::iterator &deque<ITEM>::iterator::operator++() {
  ++idx;
  return *this;
}

template <typename ITEM>
typename deque<ITEM>::iterator deque<ITEM>::iterator::operator++(int) {
  iterator x{*this};
  ++idx;
  return x;
}

template <typename ITEM>
typename deque<ITEM>::const_iterator &
deque<ITEM>::const_iterator::operator++() {
  ++idx;
  return *this;
}

template <typename ITEM>
typename deque<ITEM>::const_iterator
deque<ITEM>::const_iterator::operator++(int) {
  const_iterator x{*this};
  ++idx;
  return x;
}

template <typename ITEM> ITEM *deque<ITEM>::construct(const deque &src) {
  ITEM *a{(ITEM *)operator new(cap * sizeof(ITEM))};
  std::uninitialized_copy(src.cbegin(), src.cend(), a);
  return a;
}

template <typename ITEM> void deque<ITEM>::destruct() {
  std::destroy(begin(), end());
  operator delete(seq);
}
} // namespace core