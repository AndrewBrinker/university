/**
 * Copyright 2015 Andrew Brinker
 */

#pragma once

#include <string>
#include <vector>
using std::vector;
using std::string;

struct transition {
  string src_node;
  string dest_node;
  string expr;
};

struct transition_table {
  string             start_node;
  vector<string>     accept_nodes;
  vector<transition> transitions;
  int                num_states;
};

void crash(const std::string &);
transition_table load(const string &);

void show_transition(const transition &);
void show_table(const transition_table &);

