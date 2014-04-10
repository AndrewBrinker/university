/************************
 * File:      IntHeap.cpp
 * Author:    Andrew Brinker
 * Purpose:   Implementation declaration for IntHeap class.
 * Class:     CSE330
 * Professor: Dr. Turner
 ***********************/
#include "IntHeap.h"

// Constructor
IntHeap::IntHeap() {}

// Destructor.
IntHeap::~IntHeap() {}

// Return the current size.
unsigned int IntHeap::size() const {
	return v.size();
}

// Return whether the container is empty.
bool IntHeap::empty() const {
	return v.empty();
}

// Returns the root value.
const int& IntHeap::top() const {
	return v.front();
}

// Adds a new value.
void IntHeap::push(int x) {

	// New value x will be at index i.
	unsigned int i = v.size();
	v.push_back(x);
	while(i > 0) {
		// Compare x with its parent.
		unsigned int parent = (i-1)/2;
		if(x > v[parent]) {
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
void IntHeap::pop() {
	v[0] = v[v.size() - 1];
	v.pop_back();
	unsigned int i = 0;
	int last = v.size() - 1;
	int left = ((i+1)*2)-1;
	int right = (i+1)*2;

	// Three cases: no children, one child, two children.
	while(i < v.size() - 1) {
		// No children.
		if(left > last && right > last) {
			return;
		}
		// Left child.
		else if(left <= last && right > last) {
			if(v[i] < v[left]) {
				// Swap v[i] and v[left]
				v[i] = v[i] + v[left] - (v[left] = v[i]);
			}
			return;
		}
		// Two children
		else {
			if(v[left] > v[right]) {
				// If left is greater.
				if(v[left] > v[i]) {
					// Swap v[i] and v[left]
					v[i] = v[i] + v[left] - (v[left] = v[i]);
				}
				i = left;
			} else if(v[left] < v[right]) {
				// If right is greater.
				if(v[right] > v[i]) {
					// Swap v[i] and v[right]
					v[i] = v[i] + v[right] - (v[right] = v[i]);
				}
				i = right;
			} else {
				// If they are equal.
				if(v[left] > v[i]) {
					// Swap v[i] and v[left]
					v[i] = v[i] + v[left] - (v[left] = v[i]);
				}
				i = left;
			}
		}
		left = ((i+1)*2)-1;
		right = (i+1)*2;
	}
}
