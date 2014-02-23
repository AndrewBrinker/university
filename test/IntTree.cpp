#include <iostream>
#include <cassert>
#include <ctime>
#include <cstdlib>
// #include <set>
#include "../IntTree.h"
#include "memory.cpp"
using namespace std;

#define TREE IntTree
// #define TREE std::set<int>

void test() {

	TREE c1;                         // Create c1
	assert(c1.size() == 0);
	assert(c1.begin() == c1.end());
	c1.insert(8);                    //	(8)
	assert(c1.size() == 1);
	TREE::iterator it1 = c1.begin();
	assert(*it1 == 8);
	assert(++it1 == c1.end());
	c1.insert(10);                   //	(8,10)
	assert(c1.size() == 2);
	it1 = c1.begin();                // Set iterator to root.
	assert(*it1 == 8);
	assert(*++it1 == 10);
	assert(++it1 == c1.end());

	TREE c2;                         // Create c2.
	srand(time(NULL));
	for(int i = 0; i < 2000; ++i) {
		c2.insert(rand() % 10);
	}
	c2.insert(-1);
	assert(*(c2.begin()) == -1);     // Check begin() gives the smallest value.

	TREE c3;
	c3.insert(10);                   // (10)
	c3.insert(4);                    // ( 4,10)
	c3.insert(9);                    // ( 4, 9,10)
	TREE::iterator it3 = c3.begin();
	assert(c3.find(9) == ++it3);     // Testing find().
	c3.insert(24);                   // ( 4, 9,10,24)
	c3.insert(2);                    // ( 2, 4, 9,10,24)
	c3.insert(8);                    // ( 2, 4, 8, 9,10,24)
	assert(c3.size() == 6);
	c3.erase(it3);
	assert(c3.find(9) == c3.end()); //  ( 2, 4, 8, 10,24)

	TREE c4;
	c4.insert(10);
	c4.insert(20);
	c4.insert(15);
	c4.insert(30);
	TREE::iterator it4 = c4.begin();
	++it4;
	c4.erase(it4);
	it4 = c4.begin();
	++it4;

	TREE c5;
	TREE c6;
	c6 = c5 = c3;
	assert(c5.find(9) == c5.end());
	assert(c6.find(9) == c6.end());
	assert(c6.find(2) == c6.begin());

}

int main() {
    extern unsigned int newCalls;
	extern unsigned int deleteCalls;

    test();
	assert(newCalls == deleteCalls);

    std::cout << "All tests passed." << std::endl;
    return 0;
}