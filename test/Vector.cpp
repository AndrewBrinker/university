#include <iostream>
//#include <vector>
#include <cassert>
#include <string>
#include "../Vector.h"

//#define VECTOR std::vector<int>
#define VECTOR Vector<int>

//#define VECTOR2 std::vector<std::string>
#define VECTOR2 Vector<std::string>

int main()
{

	unsigned int zero = 0;

	// Test reserve, size and capacity functions.
	VECTOR v1;
	assert(v1.capacity() >= zero);
	assert(v1.size() == 0);
	v1.reserve(2);
	assert(v1.capacity() >= 2);
	assert(v1.size() == 0);

	// Test 2-argument constructor.
	VECTOR v2(3, 7);
	assert(v2.capacity() >= 3);
	assert(v2.size() == 3);
	assert(v2[0] == 7);
	assert(v2[1] == 7);
	assert(v2[2] == 7);

	// Test that reserve maintains existing data.
	v2.reserve(4);
	assert(v2.capacity() >= 4);
	assert(v2.size() == 3);
	assert(v2[0] == 7);
	assert(v2[1] == 7);
	assert(v2[2] == 7);

	// Test copy constructor.
	VECTOR v3(v2);
	assert(v3.capacity() >= 3);
	assert(v3.size() == 3);
	assert(v3[0] == 7);
	assert(v3[1] == 7);
	assert(v3[2] == 7);

    // Test the resize function.
	v3.resize(4, 8);
	assert(v3.capacity() >= 4);
	assert(v3.size() == 4);
	assert(v3[0] == 7);
	assert(v3[1] == 7);
	assert(v3[2] == 7);
	assert(v3[3] == 8);
	v3.resize(2);
	assert(v3.capacity() >= 4);
	assert(v3.size() == 2);
	assert(v3[0] == 7);
	assert(v3[1] == 7);
	v3.resize(3);
	assert(v3.capacity() >= 4);
	assert(v3.size() == 3);
	assert(v3[0] == 7);
	assert(v3[1] == 7);
	assert(v3[2] == 0);

	// Test the assignment operator.
	VECTOR v4;
	v4 = v3;
	assert(v4.capacity() >= 3);
	assert(v4.size() == 3);
	assert(v4[0] == 7);
	assert(v4[1] == 7);
	assert(v4[2] == 0);
	VECTOR v5(2, 9);
	v4 = v5;
	assert(v4.capacity() >= 3);
	assert(v4.size() == 2);
	assert(v4[0] == 9);
	assert(v4[1] == 9);

	// Test the pop_back function.
	v4.pop_back();
	assert(v4.capacity() >= 3);
	assert(v4.size() == 1);
	assert(v4[0] == 9);
	v4.pop_back();
	assert(v4.capacity() >= 3);
	assert(v4.size() == 0);

	// Test the push_back function.
	VECTOR v6;
	v6.push_back(100);
	assert(v6.capacity() >= 1);
	assert(v6.size() == 1);
	assert(v6[0] == 100);
	v6.push_back(101);
	assert(v6.capacity() >= 2);
	assert(v6.size() == 2);
	assert(v6[0] == 100);
	assert(v6[1] == 101);
	v6.push_back(102);
	assert(v6.capacity() >= 3);
	assert(v6.size() == 3);
	assert(v6[0] == 100);
	assert(v6[1] == 101);
	assert(v6[2] == 102);
	v6.pop_back();
	assert(v6.capacity() >= 3);
	assert(v6.size() == 2);
	assert(v6[0] == 100);
	assert(v6[1] == 101);
	v6.push_back(102);
	assert(v6.capacity() >= 3);
	assert(v6.size() == 3);
	assert(v6[0] == 100);
	assert(v6[1] == 101);
	assert(v6[2] == 102);
	v6.push_back(103);
	assert(v6.capacity() >= 4);
	assert(v6.size() == 4);
	assert(v6[0] == 100);
	assert(v6[1] == 101);
	assert(v6[2] == 102);
	assert(v6[3] == 103);

	// Test the interator.
	VECTOR v7(99, 7);
	VECTOR::iterator it = v7.begin();
	while (it != v7.end())
	{
		assert(*it == 7);
		++it;
	}

	// Test other functions.
	assert(v7.front() == 7);
	assert(v7.back() == 7);
	assert(!v7.empty());
	VECTOR v8;
	assert(v8.empty());

	//////////////////////////////////////////////////////////////
	// Repeat tests with the string datatype.
	//////////////////////////////////////////////////////////////

	// Test reserve, size and capacity functions.
	VECTOR2 s1;
	assert(s1.capacity() >= zero);
	assert(s1.size() == 0);
	s1.reserve(2);
	assert(s1.capacity() >= 2);
	assert(s1.size() == 0);

	// Test 2-argument constructor.
	VECTOR2 s2(3, "7");
	assert(s2.capacity() >= 3);
	assert(s2.size() == 3);
	assert(s2[0] == "7");
	assert(s2[1] == "7");
	assert(s2[2] == "7");

	// Test that reserve maintains existing data.
	s2.reserve(4);
	assert(s2.capacity() >= 4);
	assert(s2.size() == 3);
	assert(s2[0] == "7");
	assert(s2[1] == "7");
	assert(s2[2] == "7");

	// Test copy constructor.
	VECTOR2 s3(s2);
	assert(s3.capacity() >= 3);
	assert(s3.size() == 3);
	assert(s3[0] == "7");
	assert(s3[1] == "7");
	assert(s3[2] == "7");

    // Test the resize function.
	s3.resize(4, "8");
	assert(s3.capacity() >= 4);
	assert(s3.size() == 4);
	assert(s3[0] == "7");
	assert(s3[1] == "7");
	assert(s3[2] == "7");
	assert(s3[3] == "8");

	s3.resize(2);
	assert(s3.capacity() >= 4);
	assert(s3.size() == 2);
	assert(s3[0] == "7");
	assert(s3[1] == "7");
	s3.resize(3, "0");
	assert(s3.capacity() >= 4);
	assert(s3.size() == 3);
	assert(s3[0] == "7");
	assert(s3[1] == "7");
	assert(s3[2] == "0");

	// Test the assignment operator.
	VECTOR2 s4;
	s4 = s3;
	assert(s4.capacity() >= 3);
	assert(s4.size() == 3);
	assert(s4[0] == "7");
	assert(s4[1] == "7");
	assert(s4[2] == "0");
	VECTOR2 s5(2, "9");
	s4 = s5;
	assert(s4.capacity() >= 3);
	assert(s4.size() == 2);
	assert(s4[0] == "9");
	assert(s4[1] == "9");

	// Test the pop_back function.
	s4.pop_back();
	assert(s4.capacity() >= 3);
	assert(s4.size() == 1);
	assert(s4[0] == "9");
	s4.pop_back();
	assert(s4.capacity() >= 3);
	assert(s4.size() == 0);


	// Test the push_back function.
	VECTOR2 s6;
	s6.push_back("100");
	assert(s6.capacity() >= 1);
	assert(s6.size() == 1);
	assert(s6[0] == "100");
	s6.push_back("101");
	assert(s6.capacity() >= 2);
	assert(s6.size() == 2);
	assert(s6[0] == "100");
	assert(s6[1] == "101");
	s6.push_back("102");
	assert(s6.capacity() >= 3);
	assert(s6.size() == 3);
	assert(s6[0] == "100");
	assert(s6[1] == "101");
	assert(s6[2] == "102");
	s6.pop_back();
	assert(s6.capacity() >= 3);
	assert(s6.size() == 2);
	assert(s6[0] == "100");
	assert(s6[1] == "101");
	s6.push_back("102");
	assert(s6.capacity() >= 3);
	assert(s6.size() == 3);
	assert(s6[0] == "100");
	assert(s6[1] == "101");
	assert(s6[2] == "102");
	s6.push_back("103");
	assert(s6.capacity() >= 4);
	assert(s6.size() == 4);
	assert(s6[0] == "100");
	assert(s6[1] == "101");
	assert(s6[2] == "102");
	assert(s6[3] == "103");

	// Test the iterator.
	VECTOR2 s7(99, "7");
	VECTOR2::iterator sit = s7.begin();
	while (sit != s7.end()) {
		assert(*sit == "7");
		++sit;
	}

	// Test other functions.
	assert(s7.front() == "7");
	assert(s7.back() == "7");
	assert(!s7.empty());
	VECTOR s8;
	assert(s8.empty());

	std::cout << "All tests passed." << std::endl;
}