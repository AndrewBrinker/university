#ifndef INT_VECTOR_H
#define INT_VECTOR_H
/************************
 * File:      IntVector.h
 * Author:    Andrew Brinker
 * Purpose:   Interface declaration for IntVector class.
 * Class:     CSE330
 * Professor: Dr. Turner
 ***********************/
using namespace std;
class IntVector {
public:

// Typedefs
	typedef int * iterator;                        // Points to element.

// Copy Constructor & Assignment Operator
	IntVector(const IntVector &v);                 // Copy Constructor.
	IntVector & operator=(const IntVector &v);     // Assignment operator.

// Other Constructors
	IntVector();                                   // Base Constructor.
	IntVector(unsigned int n, int x);              // Constructor with parameters.

// Other Operators
	int & operator[](unsigned int i);              // The index operator.

// Member Functions
	unsigned int capacity() const;                 // Returns storage capacity.
	unsigned int size() const;                     // Returns size of container.
	void clear();                                  // Empties the vector.
	bool empty() const;                            // Checks if data is empty.
	void reserve(unsigned int min_cap);            // Sets capacity to min_cap.
	void resize(unsigned int min_size, int k = 0); // Sets size to n.
	int  front() const;                            // Reference to first element.
	int  back() const;                             // Reference to last element.
	void pop_back();                               // Decrements size by 1.
	void push_back(int x);                         // Increments size and puts in x.

// Iterator Functions
	iterator begin() const;                        // Returns pointer at first loc.
	iterator end() const;                          // Returns pointer at last+1 loc.

private:
	unsigned int my_capacity;                      // Capacity of the container.
	unsigned int my_size;                          // The size of the container.
	int * data;                                    // Pointer to array.
};
#endif