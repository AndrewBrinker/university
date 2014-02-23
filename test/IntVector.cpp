#include <iostream>
#include <cassert>
#include "../IntVector.h"
using namespace std;

#define VECTOR IntVector
#define ITERATOR IntVector::iterator

int main() {

	unsigned int zero = 0;
	VECTOR v1;                   // Creates v1.
	assert(v1.capacity()>=zero); // Checks that capacity is at least 0.
	assert(v1.size() == 0);      // Checks that the size is 0.
	v1.reserve(2);               // Reserves two memory slots.
	assert(v1.capacity() >= 2);  // Checks that the capacity is at least 2.
	assert(v1.size() == 0);      // Checks that the size is the same.

	VECTOR v2(3, 7);             // Creates v2 = {7,7,7}
	assert(v2.capacity() >= 3);  // Checks capacity is at least 3.
	assert(v2.size() == 3);      // Checks that size is 3.
	assert(v2[0] == 7);          // Checks that first value is 7.
	assert(v2[1] == 7);          // Checks that second value is 7.
	assert(v2[2] == 7);          // Checks that third value is 7.

	v2.reserve(4);               // Checks that reserve mantains data.
	assert(v2.capacity() >= 4);  // Checks capacity is at least 4.
	assert(v2.size() == 3);      // Checks size is still 3.
	assert(v2[0] == 7);          // Checks first value is still 7.
	assert(v2[1] == 7);          // Checks second value is still 7.
	assert(v2[2] == 7);          // Checks third value is still 7.

	VECTOR v3(v2);               // Copies v2 to v3.
	assert(v3.capacity() >= 3);  // Checks capacity is at least 3.
	assert(v3.size() == 3);      // Checks size is 3.
	assert(v3[0] == 7);          // Checks first value is 7.
	assert(v3[1] == 7);          // Checks second value is 7.
	assert(v3[2] == 7);          // Checks third value is 7.

	v3.resize(4, 8);             // Checks resize function.
	assert(v3.capacity() >= 4);  // Verifies new capacity.
	assert(v3.size() == 4);      // Verifies new size.
	assert(v3[0] == 7);          // Verifies first value is still 7.
	assert(v3[1] == 7);          // Verifies second value is still 7.
	assert(v3[2] == 7);          // Verifies third value is still 7.
	assert(v3[3] == 8);          // Verifies filler content was included.
	v3.resize(2);                // Resizes to less than current value.
	assert(v3.capacity() >= 4);  // Verifies capacity didn't change.
	assert(v3.size() == 2);      // Verifies size was changed.
	assert(v3[0] == 7);          // Verifies first value wasn't lost.
	assert(v3[1] == 7);          // Verifies second value wasn't lost.
	v3.resize(3);                // Verifies resize fills with 0 without parameter.
	assert(v3.capacity() >= 4);  // Verifies capacity is at least 4.
	assert(v3.size() == 3);      // Verifies size was increased.
	assert(v3[0] == 7);          // Verifies first value is 7.
	assert(v3[1] == 7);          // Verifies second value is 7.
	assert(v3[2] == 0);          // Verifies k defaults to 0.

	VECTOR v4;                   // Create v4.
	v4 = v3;                     // Test assignment operator.
	assert(v4.capacity() >= 3);  // Verifies capacity was copied.
	assert(v4.size() == 3);      // Verifies size was copied.
	assert(v4[0] == 7);          // Verifies content was copied.
	assert(v4[1] == 7);          // Ditto.
	assert(v4[2] == 0);          // Ditto.
	VECTOR v5(2, 9);             // Creates vector with preexisting content.
	v4 = v5;                     // Tests assignment operator.
	assert(v4.capacity() >= 3);  // Verifies capacity was copied.
	assert(v4.size() == 2);      // Verifies size was copied.
	assert(v4[0] == 9);          // Verifies content was copied.
	assert(v4[1] == 9);          // Verifies content was copied.

	v4.pop_back();               // Remove last value.
	assert(v4.capacity() >= 3);  // Checks capacity is the same.
	assert(v4.size() == 1);      // Checks size was decremented.
	assert(v4[0] == 9);          // Checks first value wasn't lost.
	v4.pop_back();               // Removes only remaining value.
	assert(v4.capacity() >= 3);  // Checks capacity is the same.
	assert(v4.size() == 0);      // Checks size is 0.
	assert(v4.empty());          // Verifies v4 is empty.

	VECTOR v6;
	v6.push_back(100);          // Increment my_size and put in content.
	assert(v6.capacity() >= 1); // Checks capacity is at least 1.
	assert(v6.size() == 1);     // Checks size is 1.
	assert(v6[0] == 100);       // Checks first value is 100.
	v6.push_back(101);          // Adds another value. v6 = {100,101}
	assert(v6.capacity() >= 2); // Checks capacity is at least 2.
	assert(v6.size() == 2);     // Checks size is 2.
	assert(v6[0] == 100);       // Checks first value is correct.
	assert(v6[1] == 101);       // Checks second value is correct.
	v6.push_back(102);          // Adds a third value.
	assert(v6.capacity() >= 3); // Checks capacity is at least 3.
	assert(v6.size() == 3);     // Checks size is 3.
	assert(v6[0] == 100);       // Checks first value is correct.
	assert(v6[1] == 101);       // Checks second value is correct.
	assert(v6[2] == 102);       // Checks third value is correct.
	v6.pop_back();              // Removes third value.
	assert(v6.capacity() >= 3); // Checks capacity is the same.
	assert(v6.size() == 2);     // Checks size is decremented.
	assert(v6[0] == 100);       // Checks first value is the same.
	assert(v6[1] == 101);       // Checks second value is the same.
	v6.push_back(102);          // Adds new third value.
	assert(v6.capacity() >= 3); // Checks capacity is the same.
	assert(v6.size() == 3);     // Checks size is incremented.
	assert(v6[0] == 100);       // Checks first value is the same.
	assert(v6[1] == 101);       // Checks second value is the same.
	assert(v6[2] == 102);       // Checks third value is correct.
	v6.push_back(103);          // Adds a fourth value.
	assert(v6.capacity() >= 4); // Checks capacity was increased.
	assert(v6.size() == 4);     // Checks size is correct.
	assert(v6[0] == 100);       // Checks first value is correct.
	assert(v6[1] == 101);       // Checks second value is correct.
	assert(v6[2] == 102);       // Checks third value is correct.
	assert(v6[3] == 103);       // Checks fourth value is correct.

	VECTOR v7(99, 7);           // Creates v7.
	ITERATOR it = v7.begin();   // Creates iterator at beginning.
	while (it != v7.end()) {    // Runs through entire vector.
		assert(*it == 7);       // Asserts all values are correct.
		++it;                   // Progresses to next cell in vector.
	}

	VECTOR v8;                  // Creates v8.
	assert(v8.empty());         // Checks v8 is empty.
	v8.push_back(20);           // Adds first value.
	v8.push_back(12);           // Adds second value.
	v8.push_back(17);           // Adds third value.
	assert(v8.front() == 20);   // Checks first value is correct.
	assert(v8.back() == 17);    // Checks last value is correct.

	VECTOR v9;                  // Creates v9.
	VECTOR v10;                 // Creates v10.
	VECTOR v11(80,2);           // Creates v11 with content.
	v9 = v10 = v11;             // Copies v11 to v10 and v9.
	assert(v10[78] == 2);       // Verifies copy worked to v10.
	assert(v9[79] == 2);        // Verifies copying worked all the way to v9.

	v9.clear();                 // Empties v9.
	assert(v9.size() == 0);     // Verifies the size is now 0.

	std::cout << "All tests passed." << std::endl;
}