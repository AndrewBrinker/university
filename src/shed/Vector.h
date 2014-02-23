#ifndef VECTOR_H
#define VECTOR_H
/************************
 * File:      Vector.h
 * Author:    Andrew Brinker
 * Purpose:   Interface declaration for Vector class.
 * Class:     CSE330
 * Professor: Dr. Turner
 ***********************/
#include <string>
using namespace std;

template <typename T>
class Vector {
public:

// Typedefs
	typedef T* iterator;                    // Points to element.

// Constructors
	Vector();                               // Base Constructor.
	Vector(const Vector &v);                // Copy Constructor.
	Vector(unsigned int n, T x);            // Constructor with parameters.

// Destructor
	~Vector();                              // Destructor.

// Operators
	Vector & operator=(const Vector &v);    // Assignment operator.
	T & operator[](unsigned int i);         // The index operator.

// Member Functions
	unsigned int capacity() const;          // Returns storage capacity.
	unsigned int size() const;              // Returns size of container.
	bool empty() const;                     // Checks if data is empty.
	void reserve(unsigned int min_cap);     // Sets capacity to min_cap.
	void resize(unsigned int min_size, T k = T()); // Sets size to n.
	T& front() const;                       // Reference to first element.
	T& back() const;                        // Reference to last element.
	void pop_back();                        // Decrements size by 1.
	void push_back(T x);                    // Increments size and puts in x.

// Iterator Functions
	iterator begin() const;                 // Returns pointer at first loc.
	iterator end() const;                   // Returns pointer at last+1 loc.

private:
	unsigned int my_capacity;               // Capacity of the container.
	unsigned int my_size;                   // The size of the container.
	T * data;                               // Pointer to array.
};

// Base Constructor.
template <typename T>
Vector<T>::Vector(): my_capacity(0), my_size(0), data(0) {}

// Copy Constructor.
template <typename T>
Vector<T>::Vector(const Vector &v) {
	my_capacity = v.capacity();
	my_size = v.size();
	data = new T[my_capacity];
	for(unsigned int i = 0; i < my_size; ++i) {
		data[i] = v.data[i];
	}
}

// Constructor with parameters.
template <typename T>
Vector<T>::Vector(unsigned int n, T x): my_capacity(n), my_size(n) {
	data = new T[n];
	for(unsigned int i = 0; i < my_size; ++i) {
		data[i] = x;
	}
}

// Destructor
template <typename T>
Vector<T>::~Vector() {
	delete[] data;
}

// Assignment operator.
template <typename T>
Vector<T> & Vector<T>::operator=(const Vector &v) {
	if(v.capacity() > my_capacity) {
		my_capacity = v.capacity();
	}
	else if(my_capacity < v.size()) {
		my_capacity = (v.size() + 1);
	}
	my_size = v.size();
	data = new T[my_capacity];
	for(unsigned int i = 0; i < my_size; ++i) {
		data[i] = v.data[i];
	}
	return *this;
}

// The index operator.
template <typename T>
T & Vector<T>::operator[](unsigned int i) {
	return data[i];
}

// Returns the amount of allocated storage capacity.
template <typename T>
unsigned int Vector<T>::capacity() const {
    return my_capacity;
}

// Returns the current size of the container.
template <typename T>
unsigned int Vector<T>::size() const {
    return my_size;
}

// Checks if data is empty.
template <typename T>
bool Vector<T>::empty() const {
	return my_size == 0;
}

// Sets storage capacity to enough for n elements.
template <typename T>
void Vector<T>::reserve(unsigned int min_cap) {
	// If no extra space needed, return.
	if(my_capacity >= min_cap) return;
	// If no internal array present, create on.
	if(data == 0) {
		data = new T[min_cap];
	} else {
		// Otherwise, make a new, larger array and copy the elements to it.
		T *old_data = data;
		data = new T[min_cap];
		for(unsigned int i = 0; i < my_size; ++i) {
			data[i] = old_data[i];
		}
		delete [] old_data;
	}
	// Set the new capacity value.
	my_capacity = min_cap;
}

// Resizes the vector to min_size with k as filler.
template <typename T>
void Vector<T>::resize(unsigned int min_size, T k) {
	// If new size is larger than current capacity, reserve more space.
	if(min_size > my_capacity) {
		reserve(min_size + 5);
	}
	// If the new size is bigger than the current size, insert the necessary
	// filler values.
	if(min_size > my_size) {
		for(unsigned int i = my_size; i < min_size; ++i) {
			data[i] = k;
		}
	}
	// Set the new size value.
	my_size = min_size;
}

// Reference to first element.
template <typename T>
T& Vector<T>::front() const {
	return data[0];
}

// Reference to last element.
template <typename T>
T& Vector<T>::back() const {
	return data[my_size - 1];
}

// Decrements size by 1.
template <typename T>
void Vector<T>::pop_back() {
	--my_size;
}

// Increments size and puts in x.
template <typename T>
void Vector<T>::push_back(T x) {
	resize(my_size + 1);
	data[my_size - 1] = x;
}

// Pointer to first element.
template <typename T>
typename Vector<T>::iterator Vector<T>::begin() const {
	return data;
}

// Pointer to one past the last element.
template <typename T>
typename Vector<T>::iterator Vector<T>::end() const {
	return data + my_size;
}
#endif