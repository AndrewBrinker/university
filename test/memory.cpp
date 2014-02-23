#ifndef MEMORY
#define MEMORY

// How to test for memory leaks in C++ programs
// that allocate memory through the new operator.
//
// Author: David Turner (of CSUSB)

#include <memory>
#include <stdlib.h>

////////////////////////////////////////////////////////////////
//                                                            //
// Override the new and delete operators to keep track of     //
// the number of times they are called.                       //
//                                                            //
// If there is no memory leak, then the number of times new   //
// is called will equal the number of times delete is called  //
// after all destructors run.                                 //
//                                                            //
////////////////////////////////////////////////////////////////

unsigned int newCalls = 0;
unsigned int deleteCalls = 0;

void * operator new(size_t size) throw(std::bad_alloc) {
	void * p = malloc(size);
	if (!p) throw std::bad_alloc();
	++newCalls;
	return p;
}

void * operator new[](size_t size) throw(std::bad_alloc) {
	void * p = malloc(size);
	if (!p) throw std::bad_alloc();
	++newCalls;
	return p;
}

void operator delete(void * p) throw() {
    ++deleteCalls;
    free(p);
}

void operator delete[](void * p) throw() {
    ++deleteCalls;
    free(p);
}

#endif