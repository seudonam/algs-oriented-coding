#pragma once
#include <algorithm>
namespace ns {
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

  using value_type = ITEM;
  using reference = value_type &;
  using const_reference = const value_type &;
  using size_type = int;

  constexpr ITEM &operator[](int i) const { return seq[(head + i) % cap]; }
  constexpr ITEM &back() const { return seq[tail]; }
  constexpr ITEM &front() const { return seq[head]; }
  constexpr int size() const { return sz; }
  constexpr int capacity() const { return cap; }
  constexpr bool empty() const { return sz == 0; }

  void reserve(int);
  template <class... Args> void push_back(Args &&...args);
  void push_back(ITEM &&item);
  void pop_back();
  template <class... Args> void push_front(Args &&...args);
  void pop_front();
  void clear();

  class iterator {
    friend class deque;

  public:
    using difference_type = std::ptrdiff_t;
    using value_type = ITEM;
    using pointer = ITEM *;
    using reference = ITEM &;
    using iterator_category = std::forward_iterator_tag;

    constexpr ITEM &operator*() const { return deck[idx]; }
    bool operator==(const iterator &rhs) const { return idx == rhs.idx; }
    bool operator!=(const iterator &rhs) const { return idx != rhs.idx; }
    iterator &operator++();
    iterator operator++(int);

  private:
    iterator(deque &deck, int idx) : deck{deck}, idx{idx} {}
    deque &deck;
    int idx;
  };

  class const_iterator {
    friend class deque;

  public:
    using difference_type = std::ptrdiff_t;
    using value_type = ITEM;
    using pointer = const ITEM *;
    using reference = const ITEM &;
    using iterator_category = std::forward_iterator_tag;

    constexpr ITEM &operator*() const { return deck[idx]; }
    bool operator==(const const_iterator &rhs) const { return idx == rhs.idx; }
    bool operator!=(const const_iterator &rhs) const { return idx != rhs.idx; }
    const_iterator &operator++();
    const_iterator operator++(int);

  private:
    const_iterator(const deque &deck, int idx) : deck{deck}, idx{idx} {}
    const deque &deck;
    int idx;
  };

  iterator begin() { return iterator(*this, 0); }
  iterator end() { return iterator(*this, sz); }
  const_iterator begin() const { return const_iterator(*this, 0); }
  const_iterator end() const { return const_iterator(*this, sz); }

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
  for (int i = 0; i < n; ++i)
    new (seq + i) ITEM();
}

template <typename ITEM>
deque<ITEM>::deque(int n, const ITEM item)
    : sz{n}, cap{n > init_cap ? n : init_cap}, head{0}, tail{sz - 1},
      seq{(ITEM *)operator new(cap * sizeof(ITEM))} {
  for (int i = 0; i < n; ++i)
    new (seq + i) ITEM(item);
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
    ITEM *next{(ITEM *)operator new(n * sizeof(ITEM))};
    for (int i = 0; i < sz; ++i) {
      new (next + i) ITEM(std::move(seq[(head + i) % cap]));
      (seq + (head + i) % cap)->~ITEM();
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
  // std::construct_at(seq + tail, std::forward<Args>(args)...);
  new (seq + tail) ITEM(std::forward<Args>(args)...);
}

// 对sz等于0和sz大于0两种情况分别处理
template <typename ITEM> void deque<ITEM>::push_back(ITEM &&item) {
  if (sz == cap)
    reserve(cap << 1);
  ++sz;
  if (sz > 1)
    if (++tail == cap)
      tail = 0;
  // std::construct_at(seq + tail, std::forward<Args>(args)...);
  new (seq + tail) ITEM(std::forward<ITEM>(item));
}

// 对sz等于1和sz大于1两种情况分别处理
template <typename ITEM> void deque<ITEM>::pop_back() {
  // std::destroy_at(seq + tail);
  (seq + tail)->~ITEM();
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
  // std::construct_at(seq + head, std::forward<Args>(args)...);
  new (seq + head) ITEM(std::forward<Args>(args)...);
}

// 对sz等于1和sz大于1两种情况分别处理
template <typename ITEM> void deque<ITEM>::pop_front() {
  // std::destroy_at(seq + head);
  (seq + head)->~ITEM();
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
  for (int i = 0; i < sz; ++i)
    new (a + i) ITEM(src[i]);
  return a;
}

template <typename ITEM> void deque<ITEM>::destruct() {
  for (int i = 0; i < sz; ++i)
    (seq + (head + i) % cap)->~ITEM();
  operator delete(seq);
}
// queue
template <typename ITEM> class queue : private deque<ITEM> {
public:
  ITEM &front() { return deque<ITEM>::front(); }
  ITEM &back() { return deque<ITEM>::back(); }
  void enqueue(const ITEM e) { deque<ITEM>::push_back(e); }
  ITEM dequeue() {
    ITEM x = deque<ITEM>::front();
    deque<ITEM>::pop_front();
    return x;
  }
};
} // namespace ns