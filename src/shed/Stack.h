#ifndef STACK_H
#define STACK_H

/*
#include "Vector.h"
#include "List.h"
*/
#include <vector>
#include <list>
#include <cassert>
using namespace std;

template <typename T, typename Container = vector<T> >
class Stack {
public:

	// Constructor/destructor.
	Stack();                    // No-Arg Constructor.
	// Stack(Stack<T> s1);         // Copy Constructor.
	~Stack();                   // Destructor.

	// Operator.
	Stack<T> operator=(Stack<T> s1); // Assignment operator.

	// Member functions
	void push(const T x);       // Adds a value.
	void pop();                 // Removes a value.
	T& top();                   // Returns the top value.
	unsigned int size() const;  // Returns the current size.
	bool empty() const;         // Tells whether the container is empty.

private:
	Container c1;               // The container being used.
};


/*-||-----------------------------+
   || Stack                       |
 *-||-----------------------------*/

/**************************************
 Constructor & Destructor
 */

// No-Arg Constructor
template <typename T, typename Container>
Stack<T,Container>::Stack() {}

// Destructor
template <typename T, typename Container>
Stack<T,Container>::~Stack() {}

// Copy Constructor.
// template <typename T, typename Container>
// Stack<T,Container>::Stack(Stack<T> s1) {
// 	assert(false);
// }

// Assignment operator.
template <typename T, typename Container>
Stack<T> Stack<T,Container>::operator=(Stack<T> s1) {
	assert(false);
}

/**************************************
 Member functions
 */

// Adds a value.
template <typename T, typename Container>
void Stack<T,Container>::push(const T x) {
	c1.push_back(x);
}

// Removes a value.
template <typename T, typename Container>
void Stack<T,Container>::pop() {
	c1.pop_back();
}

// Returns the top value.
template <typename T, typename Container>
T& Stack<T,Container>::top() {
	return c1.back();
}

// Returns the size.
template <typename T, typename Container>
unsigned int Stack<T,Container>::size() const {
	return c1.size();
}

// Empty
template <typename T, typename Container>
bool Stack<T,Container>::empty() const {
	return c1.empty();
}

#endif