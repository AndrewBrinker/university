/**
 * Copyright 2014 Andrew Brinker
 */

#ifndef LIST_H
#define LIST_H

#include <List/ListIterator.h>
#include <List/ListNode.h>

/**
 * The actual list. Composed of nodes.
 */
template <typename T>
class List {
 public:
  typedef ListIterator<T> iterator;

  List();
  ~List();

  unsigned int size() const;
  void clear();
  bool empty() const;
  T &front();
  T &back();
  void pop_back();
  void push_back(T x);
  void pop_front();
  void push_front(T x);
  void remove(const T value);

  ListIterator<T> begin() const;
  ListIterator<T> end();
  ListIterator<T> erase(ListIterator<T> position);

 private:
  unsigned int _size;
  ListNode<T> *first_link;
  ListNode<T> *last_link;
};


/**
 * List constructor. Creates an empty list.
 */
template <typename T>
List<T>::List() : _size(0), first_link(0), last_link(0) {}

/**
 * Destructor. Calls clear() to guarantee deletion of all nodes.
 */
template <typename T>
List<T>::~List() {
  clear();
}

/**
 * Gets the size of the current list.
 * @return the size of the list.
 */
template <typename T>
unsigned int List<T>::size() const {
  return _size;
}

/**
 * Checks whether the list is empty.
 * @return if the list is empty.
 */
template <typename T>
bool List<T>::empty() const {
  return _size == 0;
}

/**
 * Get a reference to the first element in the list.
 * @return a reference to the first element in the list.
 */
template <typename T>
T& List<T>::front() {
  return first_link->x;
}

/**
 * Get a reference to the last element in the list.
 * @return a reference to the last element in the list.
 */
template <typename T>
T& List<T>::back() {
  return last_link->x;
}

/**
 * Clear out all the elements in the list, making sure to delete each one, and
 * setting the member variables appropriately.
 */
template <typename T>
void List<T>::clear() {
  for (ListIterator<T> it1 = begin(); it1.link != 0; ++it1) {
    delete it1.link;
  }
  _size = 0;
  first_link = 0;
  last_link = 0;
}

/**
 * Remove the last node in the list.
 */
template <typename T>
void List<T>::pop_back() {
  ListIterator<T> last;
  last.link = last_link;
  erase(last);
}

/**
 * Add a node containing the given value to the end of the list.
 * @param x: T -> The value to be added.
 */
template <typename T>
void List<T>::push_back(T x) {
  ListNode<T> *link = new ListNode<T>(x);
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

/**
 * Remove the first node in the list.
 */
template <typename T>
void List<T>::pop_front() {
  ListIterator<T> first;
  first.link = first_link;
  erase(first);
}

/**
 * Add a node containing the given value to the front of the list.
 * @param x: T -> The value to be added.
 */
template <typename T>
void List<T>::push_front(T x) {
  ListNode<T> *link = new ListNode<T>(x);
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

/**
 * Remove any node with the given value from the list.
 * @param value: T -> The value being removed from the list.
 */
template <typename T>
void List<T>::remove(const T value) {
  ListIterator<T> it1 = begin();
  while (it1 != end()) {
    ListNode<T> current = *(it1.link);
    if (current.x == value) {
      erase(it1);
    }
    ++it1;
  }
}

/**
 * Get an iterator pointing to the first element in the list.
 */
template <typename T>
ListIterator<T> List<T>::begin() const {
  ListIterator<T> iter;
  iter.link = first_link;
  return iter;
}

/**
 * Get an iterator pointing to the last element in the list.
 */
template <typename T>
ListIterator<T> List<T>::end() {
  ListIterator<T> iter;
  iter.link = 0;
  return iter;
}

/**
 * Erase the node at the given position in the list.
 * @return an iterator to the next node in the list.
 */
template <typename T>
ListIterator<T> List<T>::erase(ListIterator<T> position) {
  ListIterator<T> it1 = position;

  ListNode<T> *link = it1.link;
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

