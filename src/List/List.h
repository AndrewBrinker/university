/*
 * Copyright 2014 Andrew Brinker
 */

#ifndef LIST_H
#define LIST_H

#include <cassert>

template <typename T>
class List;

template <typename T>
class ListIterator;

template <typename T>
class ListNode {
 public:
  explicit ListNode(T x);

  T x;
  ListNode<T> * prev;
  ListNode<T> * next;
  friend class List<T>;
  friend class ListIterator<T>;
};


template <typename T>
class ListIterator {
 public:
  typedef ListNode<T> node;

  ListIterator();
  explicit ListIterator(node temp);

  T operator*();
  ListIterator operator++();
  ListIterator operator--();
  bool operator==(const ListIterator it) const;
  bool operator!=(const ListIterator it) const;

  ListNode<T> * link;
  friend class List<T>;
  friend class ListNode<T>;
};


template <typename T>
class List {
 public:
  typedef ListIterator<T> iterator;
  typedef ListNode<T> node;

  List();
  ~List();
  List(List &c1);

  unsigned int size() const;
  void clear();
  bool empty() const;
  T& front();
  T& back();
  void pop_back();
  void push_back(T x);
  void pop_front();
  void push_front(T x);
  void remove(const T value);

  iterator begin() const;
  iterator end();
  iterator erase(iterator position);

 private:
  unsigned int _size;
  node* first_link;
  node* last_link;
};


template <typename T>
ListNode<T>::ListNode(T x):x(x), prev(0), next(0) {}


template <typename T>
ListIterator<T>::ListIterator():link(0) {}


template <typename T>
ListIterator<T>::ListIterator(node temp) {
  link = temp;
}


template <typename T>
T ListIterator<T>::operator*() {
  assert(link != 0);
  return link->x;
}


template <typename T>
ListIterator<T> ListIterator<T>::operator++() {
  link = link->next;
  return *this;
}


template <typename T>
ListIterator<T> ListIterator<T>::operator--() {
  link = link->prev;
  return *this;
}


template <typename T>
bool ListIterator<T>::operator==(const ListIterator it) const {
  return link == it.link;
}


template <typename T>
bool ListIterator<T>::operator!=(const ListIterator it) const {
  return link != it.link;
}


template <typename T>
List<T>::List() : _size(0), first_link(0), last_link(0) {}


template <typename T>
List<T>::~List() {
  clear();
}


template <typename T>
List<T>::List(List &c1) {
  iterator it1;
  for (it1 = c1.begin(); it1 != c1.end(); ++it1) {
    push_back(*it1);
  }
}


template <typename T>
unsigned int List<T>::size() const {
  return _size;
}


template <typename T>
bool List<T>::empty() const {
  return _size == 0;
}


template <typename T>
T& List<T>::front() {
  assert(first_link != 0);
  return first_link->x;
}


template <typename T>
T& List<T>::back() {
  assert(last_link != 0);
  return last_link->x;
}


template <typename T>
void List<T>::clear() {
  for (iterator it1 = begin(); it1.link != 0; ++it1) {
    delete it1.link;
  }
  _size    = 0;
  first_link = 0;
  last_link  = 0;
}


template <typename T>
void List<T>::pop_back() {
  iterator last;
  last.link = last_link;
  erase(last);
}


template <typename T>
void List<T>::push_back(T x) {
  node* link = new node(x);
  if (_size == 0) {
    first_link = link;
    last_link  = link;
  } else {
    last_link->next = link;
    link->prev = last_link;
    last_link  = link;
  }
  ++_size;
}


template <typename T>
void List<T>::pop_front() {
  iterator first;
  first.link = first_link;
  erase(first);
}


template <typename T>
void List<T>::push_front(T x) {
  node * link = new node(x);
  if (_size == 0) {
    first_link = link;
    last_link  = link;
  } else {
    first_link->prev = link;
    link->next = first_link;
    first_link = link;
  }
  ++_size;
}


template <typename T>
void List<T>::remove(const T value) {
  iterator it1 = begin();
  while (it1 != end()) {
    ListNode<T> current = *(it1.link);
    if (current.x == value) {
      erase(it1);
    }
    ++it1;
  }
}



template <typename T>
typename List<T>::iterator List<T>::begin() const {
  iterator iter;
  iter.link = first_link;
  return iter;
}


template <typename T>
typename List<T>::iterator List<T>::end() {
  iterator iter;
  iter.link = 0;
  return iter;
}


template <typename T>
typename List<T>::iterator List<T>::erase(iterator position) {
  iterator it1 = position;

  node* link = it1.link;
  if (link == first_link) {
    first_link = link->next;
  } else {
    link->prev->next = link->next;
  }
  if (link == last_link) {
    last_link = link->prev;
    it1.link = 0;
  } else {
    link->next->prev = link->prev;
    it1.link = link->next;
  }
  delete link;
  --_size;
  return it1;
}


#endif  // LIST_H
