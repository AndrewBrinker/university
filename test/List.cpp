#include <iostream>
#include <cassert>
#include <ctime>
#include <string>
#include <cstdlib>
// #include <list>
#include "../List.h"
#include "memory.cpp"
using namespace std;

#define LIST List<int>
#define LIST2 List<string>
// typedef std::list<int> LIST;
// typedef std::list<string> LIST2;

void print_list(LIST c1) {
	LIST::iterator it = c1.begin();
	std::cout << "List: ";
	while(it != c1.end()) {
		std::cout << *it << " ";
		++it;
	}
	std::cout << std::endl;
}

void list_filler(LIST &c1, int size, int range) {
	srand(time(NULL));
	for(int i = 0; i < size; ++i) {
		c1.push_back(rand() % range); // Random number between 0 and 9.
	}
}

void test() {

	/***********************
	 * INT TEST HARNESS
	 **********************/

	/*--
	 * c1
	 *--*/
	LIST c1;
	c1.push_back(3);                       // 3
	assert(c1.size()  == 1);               // 3
	assert(c1.front() == 3);               // 3
	c1.front() = 7;
	assert(c1.front() == 7);               // 7
	c1.push_back(10);                      // 7  10
	assert(c1.back()  == 10);              // 7  10
	c1.push_back(11);                      // 7  10  11
	assert(c1.back()  == 11);              // 7  10  11
	c1.push_front(2);                      // 2  7   10  11
	assert(c1.front() == 2);               // 2  7   10  11
	LIST c7(c1);                           // Test copy constructor.
	c1.remove(2);                          // 7  10  11
	LIST::iterator it = c1.begin();        // 7  10  11
	c1.erase(it);                          // 10 11
	assert(c1.size()  == 2);               // 10 11
	c1.pop_back();                         // 10
	assert(c1.size()  == 1);               // 10
	c1.pop_front();                        //
	assert(c1.size()  == 0);               //

	/*--
	 * c2
	 *--*/
	LIST c2;                               // Make c2.
	list_filler(c2,10,10);                 // Fill with 10 random values.
	c2.clear();                            // Empty the list.
	assert(c2.size()  == 0);               // Verify size is 0.
	list_filler(c2,10,10);                 // Fill with 10 random values.
	assert(c2.size()  == 10);              // Verify size is 0.

   /*--
	* c3
	*--*/
	LIST c3;                               // Make c3.
	assert(c3.size()  == 0);               // Verify it is empty.
	assert(c3.empty());                    // Same.
	c3.push_front(44);                     // 44
	assert(!c3.empty());                   // Verify it is not empty.
	assert(c3.front() == 44);              // 44
	assert(c3.back()  == 44);              // 44
	c3.push_front(33);                     // 33 44
	assert(c3.size()  == 2);               // Verify size is 2.
	assert(c3.front() == 33);              // 33 44
	assert(c3.back()  == 44);              // 33 44
	c3.push_front(22);                     // 22 33 44
	assert(c3.front() == 22);              // 22 33 44
	LIST::iterator it3 = c3.begin();       // Iterator pointing to beginning.
	assert(*it3   == 22);                  // 22 33 44
	assert(*++it3 == 33);                  // 22 33 44
	assert(*++it3 ==  44);                 // 22 33 44

	c3.pop_front();                        // 33 44
	it3 = c3.begin();                      // 33 44
	assert(*it3   == 33);                  // 33 44
	assert(*++it3 == 44);                  // 33 44
	c3.pop_back();                         // 33

	it3 = c3.begin();                      // 33
	c3.erase(it3);                         // Now it is empty.
	assert(c3.size()  == 0);               // Verify size is 0.
	assert(c3.empty());                    // Verify it is empty.


	/***********************
	 * STRING TEST HARNESS
	 **********************/

	/*--
	 * c4
	 *--*/
	LIST2 c4;                              // Make c4.
	c4.push_back("3");                     // 3
	assert(c4.size()  == 1);               // 3
	assert(c4.front() == "3");             // 3
	c4.front() = "7";
	assert(c4.front() == "7");             // 7
	c4.push_back("10");                    // 7  10
	assert(c4.back()  == "10");            // 7  10
	c4.push_back("11");                    // 7  10  11
	assert(c4.back()  == "11");            // 7  10  11
	c4.push_front("2");                    // 2  7   10  11
	assert(c4.front() == "2");             // 2  7   10  11
	LIST2 c8(c4);                          // Test copy constructor.
	c4.remove("2");                        // 7  10  11
	LIST2::iterator it4 = c4.begin();      // 7  10  11
	c4.erase(it4);                         // 10 11
	assert(c4.size()  == 2);               // 10 11
	c4.pop_back();                         // 10
	assert(c4.size()  == 1);               // 10
	c4.pop_front();                        //
	assert(c4.size()  == 0);               //

	/*--
	 * c5
	 *--*/
	LIST2 c5;                              // Make c5.
	c5.push_back("hello");                 // hello
	c5.push_back("world");                 // hello world
	c5.clear();                            // Empty the list.
	assert(c5.size()  == 0);               // Verify size is 0.
	c5.push_back("hello");                 // hello
	c5.push_back("world");                 // hello world
	assert(c5.size()  == 2);               // Verify size is 0.

	/*--
	 * c6
	 *--*/
	LIST2 c6;                              // Make c6.
	assert(c6.size()  == 0);               // Verify it is empty.
	assert(c6.empty());                    // Same.
	c6.push_front("44");                   // 44
	assert(!c6.empty());                   // Verify it is not empty.
	assert(c6.front() == "44");            // 44
	assert(c6.back()  == "44");            // 44
	c6.push_front("33");                   // 33 44
	assert(c6.size()  == 2);               // Verify size is 2.
	assert(c6.front() == "33");            // 33 44
	assert(c6.back()  == "44");            // 33 44
	c6.push_front("22");                   // 22 33 44
	assert(c6.front() == "22");            // 22 33 44
	LIST2::iterator it6 = c6.begin();      // Iterator pointing to beginning.
	assert(*it6   == "22");                // 22 33 44
	assert(*++it6 == "33");                // 22 33 44
	assert(*++it6 ==  "44");               // 22 33 44

	c6.pop_front();                        // 33 44
	it6 = c6.begin();                      // 33 44
	assert(*it6   == "33");                // 33 44
	assert(*++it6 == "44");                // 33 44
	c6.pop_back();                         // 33

	it6 = c6.begin();                      // 33
	c6.erase(it6);                         // Now it is empty.
	assert(c6.size()  == 0);               // Verify size is 0.
	assert(c6.empty());                    // Verify it is empty.
}

int main() {
    extern unsigned int newCalls;
    extern unsigned int deleteCalls;

    test();
    assert(newCalls == deleteCalls);

    std::cout << "All tests passed." << std::endl;
    return 0;
}