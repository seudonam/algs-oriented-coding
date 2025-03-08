#include <cassert>
#include <print>
#include <thread>

template <typename ITEM>
class list {
 private:
  struct node {
    ITEM item;
    node *next;
    node(ITEM item, node *next) : item{item}, next{next} {}
  };
  node *head{nullptr};
  int sz{0};

 public:
  list() {}
  ~list();

  constexpr bool empty() const { return sz == 0; }
  constexpr int size() const { return sz; }

  void push_front(const ITEM);
  void pop_front();
  void reverse();

  class iterator {
    friend class list;

   public:
    ITEM &operator*() const { return ptr->item; }
    bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
    bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
    iterator &operator++();
    iterator operator++(int);

   private:
    iterator(node *ptr) : ptr(ptr) {}
    node *ptr;
  };

  iterator begin() { return iterator(head); }
  iterator end() { return iterator(nullptr); }
};

template <typename ITEM>
list<ITEM>::~list() {
  while (head) {
    node *next{head->next};
    delete head;
    head = next;
  }
}

template <typename ITEM>
void list<ITEM>::push_front(const ITEM item) {
  node *x{new node(item, head)};
  head = x;
  sz++;
}

template <typename ITEM>
void list<ITEM>::pop_front() {
  assert(head != nullptr);
  node *x{head};
  head = head->next;
  delete x;
  sz--;
}

template <typename ITEM>
void list<ITEM>::reverse() {
  node *reverse{nullptr};
  while (head) {
    node *x{head};
    head = head->next;
    x->next = reverse;
    reverse = x;
  }
  head = reverse;
}

template <typename ITEM>
typename list<ITEM>::iterator &list<ITEM>::iterator::operator++() {
  ptr = ptr->next;
  return *this;
}

template <typename ITEM>
typename list<ITEM>::iterator list<ITEM>::iterator::operator++(int) {
  iterator x{*this};
  ptr = ptr->next;
  return x;
}

int main() {
  list<int> l;

  std::thread t1(&list<int>::push_front, &l, 1);
  std::thread t2(&list<int>::push_front, &l, 2);
  std::thread t3(&list<int>::push_front, &l, 3);
  t1.join();
  t2.join();
  t3.join();
  assert(l.size() == 3);

  for (auto e : l) std::print("{}\t", e);

  l.pop_front();
  l.pop_front();
  l.pop_front();
  assert(l.size() == 0);

  {
    std::jthread t4(&list<int>::push_front, &l, 4);
    std::jthread t5(&list<int>::push_front, &l, 5);
    std::jthread t6(&list<int>::push_front, &l, 6);
  }
  assert(l.size() == 3);

  for (auto e : l) std::print("{}\t", e);

  l.pop_front();
  l.pop_front();
  l.pop_front();
  assert(l.size() == 0);
}