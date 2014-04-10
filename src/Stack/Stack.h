#ifndef STACK_H
#define STACK_H

#include <Vector/Vector.h>

template <typename T, typename Container = Vector<T>>
class Stack {
 public:
  Stack();
  ~Stack();

  Stack<T> operator=(Stack<T> s1);

  void push(const T x);
  void pop();
  T& top();
  unsigned int size() const;
  bool empty() const;

 private:
  Container c1;
};


/**
 * Default constructor
 */
template <typename T, typename Container>
Stack<T,Container>::Stack() {}


/**
 * Default destructor
 */
template <typename T, typename Container>
Stack<T,Container>::~Stack() {}


/**
 * Push item onto the stack
 * @param: x -> The item to be added
 */
template <typename T, typename Container>
void Stack<T,Container>::push(const T x) {
  c1.push_back(x);
}


/**
 * Remove item from the stack
 */
template <typename T, typename Container>
void Stack<T,Container>::pop() {
  c1.pop_back();
}


/**
 * Get a reference to the top item.
 * @return a reference to the top item
 */
template <typename T, typename Container>
T& Stack<T,Container>::top() {
  return c1.back();
}


/**
 * Get the size of the stack
 * @return the size of the stack
 */
template <typename T, typename Container>
unsigned int Stack<T,Container>::size() const {
  return c1.size();
}


/**
 * Check whether the stack is empty
 * @return whether the stack is empty
 */
template <typename T, typename Container>
bool Stack<T,Container>::empty() const {
  return c1.empty();
}


#endif  // STACK_H
