#include <iostream>
#include <cassert>
#include <ctime>
#include <cstdlib>
// #include <queue>
#include "../IntHeap.h"
#include "memory.cpp"
using namespace std;

#define HEAP IntHeap
// #define HEAP std::priority_queue<int,std::vector<int> >

void test() {

	srand(time(NULL));
	HEAP h1;
	assert(h1.size() == 0);
	assert(h1.empty());

	h1.push(17);
	assert(h1.size() == 1);
	assert(!h1.empty());
	assert(h1.top() == 17);

	h1.push(3);
	assert(h1.size() == 2);
	assert(!h1.empty());
	assert(h1.top() == 17);

	h1.push(19);
	assert(h1.size() == 3);
	assert(h1.top() == 19);

	HEAP h2;
	h2.push(100);         // (100)
	h2.push(3);           // (100, 3)
	h2.push(50);          // (100, 3, 50)
	h2.push(17);          // (100, 17, 50, 3)
	h2.push(150);         // (150, 50, 100, 17, 3)
	h2.pop();             // (100, 17, 50, 3)
	assert(h2.top() == 100);

	HEAP h3;
	h3.push(50);
	h3.pop();
	assert(h3.size() == 0);
	assert(h3.empty());

	HEAP h4;
	h4.push(40);
	h4.push(120);
	h4.pop();
	assert(h4.size() == 1);
	assert(h4.top() == 40);
}

int main() {
    extern unsigned int newCalls;
	extern unsigned int deleteCalls;

    test();
	assert(newCalls == deleteCalls);

    std::cout << "All tests passed." << std::endl;
    return 0;
}