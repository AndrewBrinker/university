#ifndef INTHEAP_H
#define INTHEAP_H
/************************
 * File:      IntHeap.h
 * Author:    Andrew Brinker
 * Purpose:   Interface declaration for IntHeap class.
 * Class:     CSE330
 * Professor: Dr. Turner
 ***********************/
#include <vector>

class IntHeap {
public:
	IntHeap();                     // Constructor.
	~IntHeap();                    // Destructor.
	unsigned int size() const;     // Returns size.
	bool empty() const;            // Returns true if empty.
	const int& top() const;        // Returns root value.
	void push(int x);              // Adds new value.
	void pop();                    // Removes root value.

private:
	std::vector<int> v;            // Underlying container.
};

#endif