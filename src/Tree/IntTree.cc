#include "IntTree.h"
#include <cassert>
#include <iostream>
using namespace std;

/*-||-----------------------------+
   || IntTreeNode                 |
 *-||-----------------------------*/

/**************************************
 Constructor & Destructor
 */

// Constructor
IntTreeNode::IntTreeNode(int val):x(val) {}

IntTreeNode::IntTreeNode(int val,
			 IntTreeNode* new_parent,
			 IntTreeNode* new_left,
			 IntTreeNode* new_right):
			 x(val),
			 parent(new_parent),
			 left(new_left),
			 right(new_right) {}

// Destructor
IntTreeNode::~IntTreeNode() {}


/*-||-----------------------------+
   || IntTreeIterator             |
 *-||-----------------------------*/

/**************************************
 Constructor & Destructor
 */

IntTreeIterator::IntTreeIterator():node(0) {}

// Constructor
IntTreeIterator::IntTreeIterator(Node * val):node(val) {}

// Copy Constructor
IntTreeIterator::IntTreeIterator(const IntTreeIterator& it1) {
	node = it1.node;
}

// Destructor
IntTreeIterator::~IntTreeIterator() {}


/**************************************
 Operators
 */

// Dereference operator.
int IntTreeIterator::operator*() {
	assert(node->x != 0);
	return node->x;
}

IntTreeIterator IntTreeIterator::operator=(iterator it1) {
	node = it1.node;
	return *this;
}

// Comparison operator
bool IntTreeIterator::operator==(iterator it1) {
	return it1.node == node;
}

// Comparison operator
bool IntTreeIterator::operator!=(iterator it1) {
	return it1.node != node;
}

// Comparison operator.
bool IntTreeIterator::operator<(iterator it1) {
	return node < it1.node;
}

// Comparing operator.
bool IntTreeIterator::operator>(iterator it1) {
	return node > it1.node;
}

// Increment operator.
IntTreeIterator& IntTreeIterator::operator++() {
	/*********************
	 * Finds next highest value in tree
	 * using nature of the tree's structure.
	 *********************/
	// If right child.
	if(node->right != 0) {
		// Slide to the right.
		node = node->right;
		while(node->left != 0) {
			// Then move all the way to leftmost child.
			node = node->left;
		}
	} else if(node->parent == 0) {
		// If no parent, stop.
		node = 0;
	} else {
		// If you have a parent.
		if(node == node->parent->left) {
			// If parent's left child, move up one.
			node = node->parent;
		} else {
			// If parent's right child...
			while(node == node->parent->right) {
				// While right child of parent, move up.
				node = node->parent;
				if(node->parent == 0) {
					// If at the top of tree, stop.
					break;
				}
			}
			// Slide up one.
			node = node->parent;
		}
	}
	// Return self.
	return *this;
}

/*-||-----------------------------+
   || IntTree                     |
 *-||-----------------------------*/


/**************************************
 Constructors and Destructor
 */

// Constructor
IntTree::IntTree():root(0),my_size(0) {}

// Copy Constructor
IntTree::IntTree(const tree &t1) {
	assert(true);
}

// Destructor
IntTree::~IntTree() {
	// Start at root.
	Node* node = root;
	while(node != 0) {
		// While nodes still exist.
		if(node->left != 0) {
			// If left child is there, slide to the left.
			node = node->left;
		} else if(node->right != 0) {
			// Otherwise, if right child is there, slide to the right.
			node = node->right;
		} else if(node == root) {
			// If root, delete self, decrement size, and return.
			delete root;
			--my_size;
			return;
		} else {
			// If no children.
			Node* parent = node->parent;
			if(node == parent->left) {
				// If parent's left child, remove pointer to left child from parent.
				parent->left = 0;
			} else {
				// If parent's right child, remove pointer to right child from parent.
				parent->right = 0;
			}
			// Delete node.
			delete node;
			// Move up one.
			node = parent;
		}
	}
}

/**************************************
 Operators
 */

// Assignment operator
IntTree IntTree::operator=(IntTree t1) {
	// Empty anything out of existing tree.
	clear();
	// Iterator through passed-in tree, insert each value to self.
	for(iterator it1 = t1.begin(); it1 != t1.end(); ++it1) {
		insert(*it1);
	}
	// Return copy of self.
	return *this;
}

/**************************************
 Member functions
 */

// Returns an iterator to the root node.
IntTree::iterator IntTree::begin() {
	// Start at root.
	Node* node = root;
	// If tree is empty, return iterator to nothing.
	if(node == 0) { return iterator(0); }
	// Leftmost value is always smallest, so slide all the way to the left.
	while(node->left != 0) {
		node = node->left;
	}
	// Return iterator to leftmost child.
	return iterator(node);
}

// Returns an iterator to null.
IntTree::iterator IntTree::end() const {
	return iterator(0);
}

bool IntTree::empty() const {
	return my_size == 0;
}

// Adds a value to the tree.
void IntTree::insert(int x) {
	// If tree is empty, add root.
	if(root == 0) { root = new Node(x,0,0,0); }
	else {
		// Otherwise, start at root.
		Node* node = root;
		while(true) {
			// If value is less than current node.
			if(x < node->x) {
				// If no more left nodes.
				if(node->left == 0) {
					// Add node here.
					node->left = new Node(x, node, 0, 0);
					// And break.
					break;
				}
				// Slide one node to the left.
				node = node->left;
			} else if(x > node->x) {
				// If value is more than current node.
				if(node->right == 0) {
					// If no more right nodes, add node here.
					node->right = new Node(x, node, 0, 0);
					// And break.
					break;
				}
				// Slide one node to the right.
				node = node->right;
			} else {
				// If equal, verify equality.
				assert(x == node->x);
				// Duplicate values not allowed, exit.
				return;
			}
		}

	}
	// Increment size.
	++my_size;
}

void IntTree::clear() {
	// Iterate through whole tree and erase everything.
	for(iterator it1 = begin(); it1 != end(); ++it1) {
		erase(it1);
	}
}

// Removes a node from the tree.
void IntTree::erase(iterator it1) {
	Node* node   = it1.node;
	Node* parent = node->parent;
	/*********************
	 * 4 Cases:
	 * - No children.
	 * - Right child only.
	 * - Left child only.
	 * - Left & Right child.
	 *********************/
	// No children.
	if(node->left == 0 && node->right == 0) {
		if(parent == 0) {
			// If root node, tree is completely gone.
			root = 0;
		} else if(node == parent->left) {
			// If left child, remove pointer from parent.
			parent->left = 0;
		} else {
			// If right child, remove pointer from parent.
			parent->right = 0;
		}
	} else if(node->left == 0 && node->right != 0) {
		// Right child.
		if(parent == 0) {
			// If root node, right child is now root.
			root = node->right;
			root->parent = 0;
		} else {
			// If not root node, child's parent link points to my parent.
			node->right->parent = parent;
			if(node == parent->right) {
				// If I am my parent's right child, my right child is now parent's child.
				parent->right = node->right;
			} else {
				// If I am my parent's left child, my left child is now parent's child.
				parent->left = node->right;
			}
		}
	} else if(node->left != 0 && node->right == 0) {
		// Left child.
		if(parent == 0) {
			// If root node, right child is now root.
			root = node->left;
			root->parent = 0;
		} else {
			// If not root node, child's parent link points to my parent.
			node->left->parent = parent;
			if(node == parent->left) {
				// If I am my parent's left child, my left child is now parent's child.
				parent->left = node->left;
			} else {
				// If I am my parent's right child, my right child is now parent's child.
				parent->right = node->left;
			}
		}
	} else {
		// Verify that there are two children.
		assert(node->left != 0 && node->right != 0);
		Node* temp = node->right; // Temp points to right child.
		while(temp->left != 0) {
			temp = temp->left; // Move down right child's left branch until you reach second to last.
		}
		temp->left = node->left; // Last value is now my left child.
		node->left->parent = temp; // Second to last is parent.
		if(parent == 0) {
			// If root, right is now root.
			root = node->right;
			root->parent = 0;
		} else {
			// Otherwise, right's parent is my parent.
			node->right->parent = parent;
			if(node == node->parent->right) {
				// If I am my parent's right child, my child is now right child.
				parent->right = node->right;
			} else {
				// If I am my parent's left child, my child is now left child.
				node->parent->left = node->right;
			}
		}
	}
	// Delete me and decrement size.
	delete node;
	--my_size;
}

// Returns the current size of the tree.
unsigned int IntTree::size() const {
	return my_size;
}

// Returns iterator to node with desired value.
IntTree::iterator IntTree::find(const int& x) {
	iterator it1 = begin();
	// Iterate through whole tree.
	while(it1 != 0) {
		// If a value matches, return iterator to that point.
		if(*it1 == x) {
			return it1;
		}
		++it1;
	}
	// Otherwise, return iterator to nothing.
	return iterator(0);
}
