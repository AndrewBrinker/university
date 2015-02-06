/**
 * Copyright 2015 Andrew Brinker
 */

#pragma once

#include <string>
#include <vector>
using namespace std;

struct transition {
  int    src_id;
  int    dest_id;
  string expr;
};

struct transition_table {
  int                start_id;
  vector<int>        accept_ids;
  vector<transition> transitions;
};

void crash(const std::string &);
transition_table load(const string &);

void show_transition(const transition &);
void show_table(const transition_table &);

