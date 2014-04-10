/*
 * Copyright 2014 Andrew Brinker
 */

#include <shed/Stack.h>
#include <iostream>
#include <cassert>
#include <string>
#include <list>
#include <vector>
#include "memory.cpp"

typedef Stack<int> STACK;
typedef Stack<string> STACK2;
typedef Stack<int, list<int> > STACK3;
typedef Stack<string, list<string> > STACK4;

void test() {
  STACK c1;
  c1.push(10);
  c1.push(-5);
  c1.push(3);
  c1.pop();
  c1.top() = 6;
  assert(c1.top() == 6);
  assert(c1.size() == 2);
  c1.pop();
  c1.pop();
  assert(c1.empty());

  STACK2 c2;
  c2.push("10");
  c2.push("-5");
  c2.push("3");
  c2.pop();
  c2.top() = "6";
  assert(c2.top() == "6");
  assert(c2.size() == 2);
  c2.pop();
  c2.pop();
  assert(c2.empty());

  STACK3 c3;
  c3.push(10);
  c3.push(-5);
  c3.push(3);
  c3.pop();
  c3.top() = 6;
  assert(c3.top() == 6);
  assert(c3.size() == 2);
  c3.pop();
  c3.pop();
  assert(c3.empty());

  STACK4 c4;
  c4.push("10");
  c4.push("-5");
  c4.push("3");
  c4.pop();
  c4.top() = "6";
  assert(c4.top() == "6");
  assert(c4.size() == 2);
  c4.pop();
  c4.pop();
  assert(c4.empty());
}

int main() {
  extern unsigned int newCalls;
  extern unsigned int deleteCalls;

  test();
  assert(newCalls == deleteCalls);

  std::cout << "All tests passed." << std::endl;
  return 0;
}
