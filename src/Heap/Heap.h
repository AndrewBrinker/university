/*
 * Copyright 2014 Andrew Brinker
 */

#ifndef INTHEAP_H
#define INTHEAP_H

#include <Vector/Vector.h>

template <typename T>
class Heap {
 public:
  Heap();
  ~Heap();

  unsigned int size() const;
  bool empty() const;

  const T& top() const;
  void push(T x);
  void pop();

 private:
  Vector<T> v;
};

// Constructor
template <typename T>
Heap<T>::Heap() {}

// Destructor.
template <typename T>
Heap<T>::~Heap() {}

// Return the current size.
template <typename T>
unsigned int Heap<T>::size() const {
  return v.size();
}

// Return whether the container is empty.
template <typename T>
bool Heap<T>::empty() const {
  return v.empty();
}

// Returns the root value.
template <typename T>
const T& Heap<T>::top() const {
  return v.front();
}

// Adds a new value.
template <typename T>
void Heap<T>::push(T x) {
  // New value x will be at index i.
  unsigned int i = v.size();
  v.push_back(x);
  while (i > 0) {
    // Compare x with its parent.
    unsigned int parent = (i - 1) / 2;
    if (x > v[parent]) {
      // Swap x with its parent.
      v[i] = v[parent];
      v[parent] = x;
      i = parent;
    } else {
      // x is less than or equal to parent.
      break;
    }
  }
}

// Removes the root value.
template <typename T>
void Heap<T>::pop() {
  v[0] = v[v.size() - 1];
  v.pop_back();
  unsigned int i = 0;
  int last = v.size() - 1;
  int left = ((i + 1) * 2) - 1;
  int right = (i + 1) * 2;

  // Three cases: no children, one child, two children.
  while (i < v.size() - 1) {
    // No children.
    if (left > last && right > last) {
      return;
    } else if (left <= last && right > last) {
      // One child
      if (v[i] < v[left]) {
        // Swap v[i] and v[left]
        v[i] = v[i] + v[left] - (v[left] = v[i]);
      }
      return;
    } else {
      // Two children
      if (v[left] > v[right]) {
        // If left is greater.
        if (v[left] > v[i]) {
          // Swap v[i] and v[left]
          v[i] = v[i] + v[left] - (v[left] = v[i]);
        }
        i = left;
      } else if (v[left] < v[right]) {
        // If right is greater.
        if (v[right] > v[i]) {
          // Swap v[i] and v[right]
          v[i] = v[i] + v[right] - (v[right] = v[i]);
        }
        i = right;
      } else {
        // If they are equal.
        if (v[left] > v[i]) {
          // Swap v[i] and v[left]
          v[i] = v[i] + v[left] - (v[left] = v[i]);
        }
        i = left;
      }
    }
    left = ((i + 1) * 2) - 1;
    right = (i + 1) * 2;
  }
}


#endif

