#ifndef INTTREE_H
#define INTTREE_H
/************************
* File:      IntTree.h
* Author:    Andrew Brinker
* Purpose:   Interface declaration for IntTree class.
* Class:     CSE330
* Professor: Dr. Turner
 ***********************/
using namespace std;

class IntTreeIterator;
class IntTree;

class IntTreeNode {
public:

	// Typedefs
	typedef IntTreeNode Node;
	typedef IntTreeIterator iterator;
	typedef IntTree tree;

private:

	// Constructor & Destructor.
	IntTreeNode(int val);              // Default Constructor.
	IntTreeNode(int val,
				IntTreeNode* new_parent,
				IntTreeNode* new_left,
				IntTreeNode* new_right); // Constructor.
	~IntTreeNode();                    // Destructor.


	int x;                             // Value stored at node.
	IntTreeNode* parent;               // Pointer to parent node.
	IntTreeNode* left;                 // Left child node.
	IntTreeNode* right;                // Right child node.

	friend class IntTreeIterator;
	friend class IntTree;
};

class IntTreeIterator {
public:

	// Typedefs
	typedef IntTreeNode Node;
	typedef IntTreeIterator iterator;
	typedef IntTree tree;

public:

	// Constructor & Destructor.
	IntTreeIterator();
	IntTreeIterator(Node * node);         // Constructor.
	IntTreeIterator(const IntTreeIterator& it1); // Copy Constructor.
	~IntTreeIterator();                   // Destructor.

	// Operators
	int  operator*();                     // Dereference operator.
	bool operator==(iterator it1);        // Checks if two iterators are equal.
	bool operator!=(iterator it1);        // Checks if two ierators are not equal.
	IntTreeIterator operator=(iterator it1);
	IntTreeIterator& operator++();        // Increment operator.
	bool operator<(iterator it1);         // Less than.
	bool operator>(iterator it1);         // Greater than.

private:

	Node* node;                           // What the iterator is pointing to.

	friend class IntTreeNode;
	friend class IntTree;

};

class IntTree {
public:

	// Typedefs
	typedef IntTreeNode Node;
	typedef IntTreeIterator iterator;
	typedef IntTree tree;

	// Constructor & Destructor.
	IntTree();                           // Constructor.
	IntTree(const tree &t1);             // Copy Constructor.
	~IntTree();                          // Destructor.

	// Operators
	IntTree operator=(IntTree t1);       // Assignment operator.

	// Member functions
	iterator begin();                    // Returns an iterator to the root node.
	iterator end() const;                // Returns an iterator to null.
	bool empty() const;                  // Checks whether the tree is empty.
	void insert(int x);                  // Adds a value to the tree.
	void clear();                        // Empties the tree.
	void erase(iterator it1);            // Removes a value from the tree.
	unsigned int size() const;           // Returns the current size of the tree.
	iterator find(const int& x);         // Returns iterator to node with desired value.


private:

	Node * root;
	unsigned int my_size;

	friend class IntTreeNode;
	friend class IntTreeIterator;

};

#endif