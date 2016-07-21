/**
 * Copyright 2014 Andrew Brinker
 */

#ifndef LIST_NODE_H
#define LIST_NODE_H

/**
 * The individual nodes of a List. Each has a value `x`, and two pointers
 * `next` and `prev`, respectively.
 */
template <typename T>
class ListNode {
    public:
        explicit ListNode(T x);

        T x;
        ListNode<T> *prev;
        ListNode<T> *next;
};

/**
 * Constructor for ListNode. Populates the node with the given value and two
 * null pointers.
 * @param x: T -> The value of the node.
 */
template <typename T>
ListNode<T>::ListNode(T x) : x(x), prev(0), next(0) {}

#endif  // LIST_NODE_H

