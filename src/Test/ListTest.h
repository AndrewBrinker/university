/*
 * Copyright 2014 Andrew Brinker
 */

#ifndef LISTTEST_H
#define LISTTEST_H

#include <List/List.h>
#include <cassert>
#include <ctime>
#include <string>
#include <cstdlib>
#include <iostream>

void list_filler(List<int> &c1, int size, int range) {
    srand(time(NULL));
    for (int i = 0; i < size; ++i) {
        c1.push_back(rand() % range);
    }
}

void listTest() {
    List<int> c1;
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

    List<int> c2;
    list_filler(c2, 10, 10);
    c2.clear();
    assert(c2.size()  == 0);
    list_filler(c2, 10, 10);
    assert(c2.size()  == 10);

    List<int> c3;
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
    List<int>::iterator it3 = c3.begin();
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

    printf("ListTest: All tests passed.\n");
}

#endif  // LISTTEST_H
