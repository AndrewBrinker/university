/**
 * Copyright 2014 Andrew Brinker
 */

#ifndef LIST_ITERATOR_H
#define LIST_ITERATOR_H

#include <List/ListNode.h>

/**
 * Used for traversing a list of some type.
 */
template <typename T>
class ListIterator {
    public:
        ListIterator();
        explicit ListIterator(ListNode<T> temp);

        T operator*();
        ListIterator operator++();
        ListIterator operator--();
        bool operator==(const ListIterator it) const;
        bool operator!=(const ListIterator it) const;

        ListNode<T> *link;
};

/**
 * Empty constructor. Points to nothing.
 */
template <typename T>
ListIterator<T>::ListIterator() : link(0) {}

/**
 * Normal constructor. Points to the given node.
 * @param temp: ListNode -> The node being pointed to.
 */
template <typename T>
ListIterator<T>::ListIterator(ListNode<T> temp) : link(temp) {}

/**
 * Dereference operator.
 * @return the thing being pointed to.
 */
template <typename T>
T ListIterator<T>::operator*() {
    return link->x;
}

/**
 * Increment operator.
 * @return iterator to the next element.
 */
template <typename T>
ListIterator<T> ListIterator<T>::operator++() {
    link = link->next;
    return *this;
}

/**
 * Decrement operator.
 * @return iterator to the previous element.
 */
template <typename T>
ListIterator<T> ListIterator<T>::operator--() {
    link = link->prev;
    return *this;
}

/**
 * Equality operator.
 * @param it: ListIterator -> The iterator being compared to.
 * @return whether the two iterators point to the same thing.
 */
template <typename T>
bool ListIterator<T>::operator==(const ListIterator it) const {
    return link == it.link;
}

/**
 * Inequality operator.
 * @param it: ListIterator -> The iterator being compared to.
 * @return whether the two iterator do not point to the same thing.
 */
template <typename T>
bool ListIterator<T>::operator!=(const ListIterator it) const {
    return link != it.link;
}

#endif  // LIST_ITERATOR_H

