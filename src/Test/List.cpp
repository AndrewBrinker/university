/*
 * Copyright 2014 Andrew Brinker
 */

#include <shed/List.h>
#include <iostream>
#include <cassert>
#include <ctime>
#include <string>
#include <cstdlib>
#include "memory.cpp"

void print_list(LIST c1) {
  LIST::iterator it = c1.begin();
  std::cout << "List: ";
  while (it != c1.end()) {
    std::cout << *it << " ";
    ++it;
  }
  std::cout << std::endl;
}

void list_filler(LIST &c1, int size, int range) {
  srand(time(NULL));
  for (int i = 0; i < size; ++i) {
    c1.push_back(rand() % range);
  }
}

void test() {
  LIST c1;
  c1.push_back(3);
  assert(c1.size()  == 1);
  assert(c1.front() == 3);
  c1.front() = 7;
  assert(c1.front() == 7);
  c1.push_back(10);
  assert(c1.back()  == 10);
  c1.push_back(11);
  assert(c1.back()  == 11);
  c1.push_front(2);
  assert(c1.front() == 2);
  LIST c7(c1);
  c1.remove(2);
  LIST::iterator it = c1.begin();
  c1.erase(it);
  assert(c1.size()  == 2);
  c1.pop_back();
  assert(c1.size()  == 1);
  c1.pop_front();
  assert(c1.size()  == 0);

  LIST c2;
  list_filler(c2, 10, 10);
  c2.clear();
  assert(c2.size()  == 0);
  list_filler(c2, 10, 10);
  assert(c2.size()  == 10);

  LIST c3;
  assert(c3.size()  == 0);
  assert(c3.empty());
  c3.push_front(44);
  assert(!c3.empty());
  assert(c3.front() == 44);
  assert(c3.back()  == 44);
  c3.push_front(33);
  assert(c3.size()  == 2);
  assert(c3.front() == 33);
  assert(c3.back()  == 44);
  c3.push_front(22);
  assert(c3.front() == 22);
  LIST::iterator it3 = c3.begin();
  assert(*it3   == 22);
  assert(*++it3 == 33);
  assert(*++it3 ==  44);

  c3.pop_front();
  it3 = c3.begin();
  assert(*it3   == 33);
  assert(*++it3 == 44);
  c3.pop_back();

  it3 = c3.begin();
  c3.erase(it3);
  assert(c3.size()  == 0);
  assert(c3.empty());

  LIST2 c4;
  c4.push_back("3");
  assert(c4.size()  == 1);
  assert(c4.front() == "3");
  c4.front() = "7";
  assert(c4.front() == "7");
  c4.push_back("10");
  assert(c4.back()  == "10");
  c4.push_back("11");
  assert(c4.back()  == "11");
  c4.push_front("2");
  assert(c4.front() == "2");
  LIST2 c8(c4);
  c4.remove("2");
  LIST2::iterator it4 = c4.begin();
  c4.erase(it4);
  assert(c4.size()  == 2);
  c4.pop_back();
  assert(c4.size()  == 1);
  c4.pop_front();
  assert(c4.size()  == 0);

  LIST2 c5;
  c5.push_back("hello");
  c5.push_back("world");
  c5.clear();
  assert(c5.size()  == 0);
  c5.push_back("hello");
  c5.push_back("world");
  assert(c5.size()  == 2);

  LIST2 c6;
  assert(c6.size()  == 0);
  assert(c6.empty());
  c6.push_front("44");
  assert(!c6.empty());
  assert(c6.front() == "44");
  assert(c6.back()  == "44");
  c6.push_front("33");
  assert(c6.size()  == 2);
  assert(c6.front() == "33");
  assert(c6.back()  == "44");
  c6.push_front("22");
  assert(c6.front() == "22");
  LIST2::iterator it6 = c6.begin();
  assert(*it6   == "22");
  assert(*++it6 == "33");
  assert(*++it6 ==  "44");

  c6.pop_front();
  it6 = c6.begin();
  assert(*it6   == "33");
  assert(*++it6 == "44");
  c6.pop_back();

  it6 = c6.begin();
  c6.erase(it6);
  assert(c6.size()  == 0);
  assert(c6.empty());
}

int main() {
    extern unsigned int newCalls;
    extern unsigned int deleteCalls;

    test();
    assert(newCalls == deleteCalls);

    std::cout << "All tests passed." << std::endl;
    return 0;
}
