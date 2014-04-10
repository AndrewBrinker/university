#ifndef LIST_H
#define LIST_H
/************************
 * File:      List.h
 * Author:    Andrew Brinker
 * Purpose:   Declaration for List class.
 * Class:     CSE330
 * Professor: Dr. Turner
 ***********************/
#include <cassert>
using namespace std;

template <typename T>
class List;

template<typename T>
class ListIterator;

// The individual nodes in the list.
template <typename T>
class ListNode {
public:

	// Constructor & Destructor
	ListNode(T x);                      // Base constructor.

	T x;                                // The value in the cell.
	ListNode<T> * prev;                 // Pointer to previous cell.
	ListNode<T> * next;                 // Pointer to next cell.
	friend class List<T>;               // Declares List as friend.
	friend class ListIterator<T>;       // Makes ListIterator friend.
};



// The iterator.
template <typename T>
class ListIterator {
public:

	typedef ListNode<T> node;           // The nodes.

	// Constructor
	ListIterator();                     // Base constructor.
	ListIterator(node temp);            // Constructor that takes param.

	// Operator.
	T operator*();                      // Dereference operator.
	ListIterator operator++();          // Increment operator.
	ListIterator operator--();          // Decrement operator.
	bool operator==(const ListIterator it) const; // Equality comparion.
	bool operator!=(const ListIterator it) const; // Opposite.

	ListNode<T> * link;                 // Pointer to node.
	friend class List<T>;               // Declares List as friend.
	friend class ListNode<T>;           // Makes ListNode a friend.
};



// The list.
template <typename T>
class List {
public:

	// Typedefs
	typedef ListIterator<T> iterator;      // Points to node.
	typedef ListNode<T> node;              // The nodes.

	// Constructor and Destructor
	List();                                // Base Constructor.
	~List();                               // Destructor.
	List(List &c1);                        // Copy Constructor.

	// Operators
	List<T> operator=(List<T> c1);         // Assignment operator.

	// Member functions.
	unsigned int size() const;             // Returns size of container.
	void clear();                          // Empties the list.
	bool empty() const;                    // Checks if data is empty.
	T& front();                            // Reference to first element.
	T& back();                             // Reference to last element.
	void pop_back();                       // Decrements size by 1.
	void push_back(T x);                   // Increments size and puts in x.
	void pop_front();                      // Decrements size by 1.
	void push_front(T x);                  // Increments size and puts in x.
	void remove(const T value);            // Remove elements that match value.

	// Iterator Functions
	iterator begin() const;                // Returns pointer at first loc.
	iterator end();                        // Returns pointer at last+1 loc.
	iterator erase(iterator position);     // Removes a single element.

private:
	unsigned int my_size;                  // The current size of the list.
	node* first_link;                      // Pointer to the very first element.
	node* last_link;                       // Pointer to last element.
};

/*-||-----------------------------+
   || ListNode                    |
 *-||-----------------------------*/

// Constructor.
template <typename T>
ListNode<T>::ListNode(T x):x(x), prev(0), next(0) {}

/*-||-----------------------------+
   || ListIterator                |
 *-||-----------------------------*/

// Constructor.
template <typename T>
ListIterator<T>::ListIterator():link(0) {}

template <typename T>
ListIterator<T>::ListIterator(node temp) {
	link = temp;
}

// Pointer operator.
template <typename T>
T ListIterator<T>::operator*() {
	assert(link != 0);
	return link->x;
}

// Increment operator.
template <typename T>
ListIterator<T> ListIterator<T>::operator++() {
	link = link->next;
	return *this;
}

// Decrement operator.
template <typename T>
ListIterator<T> ListIterator<T>::operator--() {
	link = link->prev;
	return *this;
}

// Equality comparison.
template <typename T>
bool ListIterator<T>::operator==(const ListIterator it) const {
	if(link == it.link) {
		return true;
	}
	return false;
}

// Opposite.
template <typename T>
bool ListIterator<T>::operator!=(const ListIterator it) const {
	if(link == it.link) {
		return false;
	}
	return true;
}

/*-||-----------------------------+
   || List                        |
 *-||-----------------------------*/

/**************************************
 Constructor & Destructor.
 */

// Base constructor.
template <typename T>
List<T>::List():my_size(0),first_link(0),last_link(0) {}

// Destructor
template <typename T>
List<T>::~List() {
	clear();
}

// Copy Constructor
template <typename T>
List<T>::List(List &c1) {
	iterator it1;
	for(it1 = c1.begin(); it1 != c1.end(); ++it1) {
		push_back(*it1);
	}
}


/**************************************
 Operators.
 */

// Assignment operator.
template <typename T>
List<T> List<T>::operator=(List<T> c1) {
	assert(false);
}

/**************************************
 Member functions.
 */

// Returns size of container.
template <typename T>
unsigned int List<T>::size() const {
	return my_size;
}

// Checks if empty.
template <typename T>
bool List<T>::empty() const {
	if(my_size == 0) {
		return true;
	}
	return false;
}

// Reference to first element.
template <typename T>
T& List<T>::front() {
	assert(first_link != 0);
	return first_link->x;
}

// Reference to last element.
template <typename T>
T& List<T>::back() {
	assert(last_link != 0);
	return last_link->x;
}

// Empties the list.
template <typename T>
void List<T>::clear() {
	for(iterator it1 = begin(); it1.link != 0; ++it1) {
		delete it1.link;
	}
	my_size    = 0;
	first_link = 0;
	last_link  = 0;
}

// Decrement size by 1.
template <typename T>
void List<T>::pop_back() {
	iterator last;
	last.link = last_link;
	erase(last);
}

// Increment size by one and put in x.
template <typename T>
void List<T>::push_back(T x) {
	node* link = new node(x);
	if(my_size == 0) {
		first_link = link;
		last_link  = link;
	}
	else {
		last_link->next = link;
		link->prev = last_link;
		last_link  = link;
	}
	++my_size;
}

// Decrement size by 1.
template <typename T>
void List<T>::pop_front() {
	iterator first;
	first.link = first_link;
	erase(first);
}

// Increments size and puts in x.
template <typename T>
void List<T>::push_front(T x) {
	node * link = new node(x);
	if(my_size == 0) {
		first_link = link;
		last_link  = link;
	}
	else {
		first_link->prev = link;
		link->next = first_link;
		first_link = link;
	}
	++my_size;
}

// Remove elements that match value.
template <typename T>
void List<T>::remove(const T value) {
	iterator it1 = begin();
	while(it1 != end()) {
		ListNode<T> current = *(it1.link);
		if(current.x == value) {
			erase(it1);
		}
		++it1;
	}
}


/**************************************
 Iterator functions.
 */

// Returns pointer at first loc.
template <typename T>
typename List<T>::iterator List<T>::begin() const {
	iterator iter;
	iter.link = first_link;
	return iter;
}

// Returns pointer at last+1 loc.
template <typename T>
typename List<T>::iterator List<T>::end() {
	iterator iter;
	iter.link = 0;
	return iter;
}

// Removes a single element.
template <typename T>
typename List<T>::iterator List<T>::erase (iterator position ) {
	iterator it1 = position;

	node* link = it1.link;
	if(link == first_link) {
		first_link = link->next;
	}else {
		link->prev->next = link->next;
	}
	if(link == last_link) {
		last_link = link->prev;
		it1.link = 0;
	}else {
		link->next->prev = link->prev;
		it1.link = link->next;
	}
	delete link;
	--my_size;
	return it1;
}
#endif