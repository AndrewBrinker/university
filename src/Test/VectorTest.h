/*
 * Copyright 2014 Andrew Brinker
 */

#ifndef VECTORTEST_H
#define VECTORTEST_H

#include <Vector/Vector.h>
#include <cassert>
#include <string>
#include <cstdio>

void vectorTest() {
  Vector<int> v1;
  assert(v1.capacity() >= 0);
  assert(v1.size() == 0);
  v1.reserve(2);
  assert(v1.capacity() >= 2);
  assert(v1.size() == 0);

  Vector<int> v2(3, 7);
  assert(v2.capacity() >= 3);
  assert(v2.size() == 3);
  assert(v2[0] == 7);
  assert(v2[1] == 7);
  assert(v2[2] == 7);

  v2.reserve(4);
  assert(v2.capacity() >= 4);
  assert(v2.size() == 3);
  assert(v2[0] == 7);
  assert(v2[1] == 7);
  assert(v2[2] == 7);

  Vector<int> v3(v2);
  assert(v3.capacity() >= 3);
  assert(v3.size() == 3);
  assert(v3[0] == 7);
  assert(v3[1] == 7);
  assert(v3[2] == 7);

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

  Vector<int> v4;
  v4 = v3;
  assert(v4.capacity() >= 3);
  assert(v4.size() == 3);
  assert(v4[0] == 7);
  assert(v4[1] == 7);
  assert(v4[2] == 0);
  Vector<int> v5(2, 9);
  v4 = v5;
  assert(v4.capacity() >= 3);
  assert(v4.size() == 2);
  assert(v4[0] == 9);
  assert(v4[1] == 9);

  v4.pop_back();
  assert(v4.capacity() >= 3);
  assert(v4.size() == 1);
  assert(v4[0] == 9);
  v4.pop_back();
  assert(v4.capacity() >= 3);
  assert(v4.size() == 0);

  Vector<int> v6;
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

  Vector<int> v7(99, 7);
  Vector<int>::iterator it = v7.begin();
  while (it != v7.end()) {
    assert(*it == 7);
    ++it;
  }

  assert(v7.front() == 7);
  assert(v7.back() == 7);
  assert(!v7.empty());
  Vector<int> v8;
  assert(v8.empty());

  Vector<std::string> s1;
  assert(s1.capacity() >= 0);
  assert(s1.size() == 0);
  s1.reserve(2);
  assert(s1.capacity() >= 2);
  assert(s1.size() == 0);

  Vector<std::string> s2(3, "7");
  assert(s2.capacity() >= 3);
  assert(s2.size() == 3);
  assert(s2[0] == "7");
  assert(s2[1] == "7");
  assert(s2[2] == "7");

  s2.reserve(4);
  assert(s2.capacity() >= 4);
  assert(s2.size() == 3);
  assert(s2[0] == "7");
  assert(s2[1] == "7");
  assert(s2[2] == "7");

  Vector<std::string> s3(s2);
  assert(s3.capacity() >= 3);
  assert(s3.size() == 3);
  assert(s3[0] == "7");
  assert(s3[1] == "7");
  assert(s3[2] == "7");

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

  Vector<std::string> s4;
  s4 = s3;
  assert(s4.capacity() >= 3);
  assert(s4.size() == 3);
  assert(s4[0] == "7");
  assert(s4[1] == "7");
  assert(s4[2] == "0");
  Vector<std::string> s5(2, "9");
  s4 = s5;
  assert(s4.capacity() >= 3);
  assert(s4.size() == 2);
  assert(s4[0] == "9");
  assert(s4[1] == "9");

  s4.pop_back();
  assert(s4.capacity() >= 3);
  assert(s4.size() == 1);
  assert(s4[0] == "9");
  s4.pop_back();
  assert(s4.capacity() >= 3);
  assert(s4.size() == 0);

  Vector<std::string> s6;
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

  Vector<std::string> s7(99, "7");
  Vector<std::string>::iterator sit = s7.begin();
  while (sit != s7.end()) {
    assert(*sit == "7");
    ++sit;
  }

  assert(s7.front() == "7");
  assert(s7.back() == "7");
  assert(!s7.empty());
  Vector<int> s8;
  assert(s8.empty());

  printf("VectorTest: All tests passed.\n");
}

#endif  // VECTORTEST_H
