/************************
 * File:      IntVector.cpp
 * Author:    Andrew Brinker
 * Purpose:   Implementation declaration for IntVector class.
 * Class:     CSE330
 * Professor: Dr. Turner
 ***********************/
#include "IntVector.h"
#include <cassert>
using namespace std;


/**************************************
 Copy Constructor & Assignment Operator
 */

// Copy Constructor.
IntVector::IntVector(const IntVector &v) {
	my_capacity = v.capacity();
	my_size = v.size();
	data = new int[my_capacity];
	for(unsigned int i = 0; i < my_size; ++i) {
		data[i] = v.data[i];
	}
}

// Assignment operator.
IntVector & IntVector::operator=(const IntVector &v) {
	if(v.capacity() > my_capacity) {
		my_capacity = v.capacity();
	} else if(my_capacity < v.size()) {
		my_capacity = (v.size() + 1);
	}
	my_size = v.size();
	data = new int[my_capacity];
	for(unsigned int i = 0; i < my_size; ++i) {
		data[i] = v.data[i];
	}
	return *this;
}


/**************************************
 Other Constructors
 */

// Base Constructor.
IntVector::IntVector(): my_capacity(0), my_size(0), data(0) {}

// Constructor with parameters.
IntVector::IntVector(unsigned int n, int x): my_capacity(n), my_size(n) {
	data = new int[n];
	for(unsigned int i = 0; i < my_size; ++i) {
		data[i] = x;
	}
}


/**************************************
 Other Operators
 */

// The index operator.
int & IntVector::operator[](unsigned int i) {
	return data[i];
}


/**************************************
 Member Functions
 */

// Returns the amount of allocated storage capacity.
unsigned int IntVector::capacity() const {
    return my_capacity;
}

// Returns the current size of the container.
unsigned int IntVector::size() const {
    return my_size;
}

// Empties the vector.
void IntVector::clear() {
	my_size = 0;
}

// Checks if data is empty.
bool IntVector::empty() const {
	return my_size == 0;
}

// Sets storage capacity to enough for n elements.
void IntVector::reserve(unsigned int min_cap) {
	if(data == 0) {
		data = new int[min_cap];
		my_capacity = min_cap;
	} else if(my_capacity >= min_cap) {
		return;
	} else {
		int *old_data = data;
		data = new int[min_cap];
		for(unsigned int i = 0; i < my_size; ++i) {
			data[i] = old_data[i];
		}
		delete [] old_data;
		my_capacity = min_cap;
	}
}

// Resizes the vector to min_size with k as filler.
void IntVector::resize(unsigned int min_size, int k) {
	if(min_size > my_capacity) {
		reserve(min_size + 5);
	}
	if(min_size > my_size) {
		int old_size = my_size;
		my_size = min_size;
		for(unsigned int i = old_size; i < my_size; ++i) {
			data[i] = k;
		}
	} else if(min_size < my_size) {
		my_size = min_size;
	}
}

// Reference to first element.
int IntVector::front() const {
	return data[0];
}

// Reference to last element.
int IntVector::back() const {
	return data[my_size - 1];
}

// Decrements size by 1.
void IntVector::pop_back() {
	--my_size;
}

// Increments size and puts in x.
void IntVector::push_back(int x) {
	resize(my_size + 1);
	data[my_size - 1] = x;
}


/**************************************
 Iterator Functions
 */

// Returns iterator to first spot.
IntVector::iterator IntVector::begin() const {
	return data;
}

// Returns iterator to one past last spot.
IntVector::iterator IntVector::end() const {
	return data + my_size;
}