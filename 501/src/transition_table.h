/**
 * Copyright 2015 Andrew Brinker
 */

#pragma once

#include <string>
#include <vector>
#include <map>
using std::vector;
using std::string;
using std::map;

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
  map<string, int>   labels;
};

void crash(const string &);
transition_table load(const string &);
string fa_to_regex(transition_table);

void show_transition(const transition &);
void show_table(const transition_table &);

