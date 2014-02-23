#include <iostream>
#include <cassert>
#include <string>
#include "../Stack.h"
#include <list>
#include <vector>
/*
#include "../List/List.h"
#include "../Vector/Vector.h"
*/
#include "memory.cpp"
using namespace std;

typedef Stack<int> STACK;
typedef Stack<string> STACK2;
typedef Stack<int, list<int> > STACK3;
typedef Stack<string, list<string> > STACK4;

void test() {

	/******************************
	 * INT & VECTOR TEST HARNESS
	 *****************************/
	STACK c1;                             // Create c1.
	c1.push(10);                          // [10]
	c1.push(-5);                          // [10,-5]
	c1.push(3);                           // [10,-5,3]
	c1.pop();                             // [10,-5]
	c1.top() = 6;                         // [10,6]
	assert(c1.top() == 6);                // Verify 6 is the top value.
	assert(c1.size() == 2);               // Verify size is 2.
	c1.pop();                             // [10]
	c1.pop();                             // []
	assert(c1.empty());                   // Verify c1 is empty.

	/******************************
	 * STRING & VECTOR TEST HARNESS
	 *****************************/
	STACK2 c2;                            // Create c1.
	c2.push("10");                        // [10]
	c2.push("-5");                        // [10,-5]
	c2.push("3");                         // [10,-5,3]
	c2.pop();                             // [10,-5]
	c2.top() = "6";                       // [10,6]
	assert(c2.top() == "6");              // Verify 6 is the top value.
	assert(c2.size() == 2);               // Verify size is 2.
	c2.pop();                             // [10]
	c2.pop();                             // []
	assert(c2.empty());                   // Verify c1 is empty.

	/******************************
	 * INT & LIST TEST HARNESS
	 *****************************/
	STACK3 c3;                             // Create c1.
	c3.push(10);                          // [10]
	c3.push(-5);                          // [10,-5]
	c3.push(3);                           // [10,-5,3]
	c3.pop();                             // [10,-5]
	c3.top() = 6;                         // [10,6]
	assert(c3.top() == 6);                // Verify 6 is the top value.
	assert(c3.size() == 2);               // Verify size is 2.
	c3.pop();                             // [10]
	c3.pop();                             // []
	assert(c3.empty());                   // Verify c1 is empty.

	/******************************
	 * STRING & LIST TEST HARNESS
	 *****************************/
	STACK4 c4;                            // Create c1.
	c4.push("10");                        // [10]
	c4.push("-5");                        // [10,-5]
	c4.push("3");                         // [10,-5,3]
	c4.pop();                             // [10,-5]
	c4.top() = "6";                       // [10,6]
	assert(c4.top() == "6");              // Verify 6 is the top value.
	assert(c4.size() == 2);               // Verify size is 2.
	c4.pop();                             // [10]
	c4.pop();                             // []
	assert(c4.empty());                   // Verify c1 is empty.
}

int main() {
    extern unsigned int newCalls;
    extern unsigned int deleteCalls;

    test();
    assert(newCalls == deleteCalls);

    std::cout << "All tests passed." << std::endl;
    return 0;
}