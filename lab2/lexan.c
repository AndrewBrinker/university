/*
 * Author:    Andrew Brinker
 * Course:    CSE 570, Compilers
 * Professor: Dr. Ernesto Gomez
 * Term:      Winter, 2013-2014
 * License:   All rights reserved (don't copy this for your own homework, man)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define EXPRESSION    "(a|b)*abb"
#define TABLE         {{1,0}, {1,2}, {1, 3}, {1, 0}}
#define ALPHABET      {'a', 'b'}
#define START_STATE   0
#define HALT_STATE    3
#define STATE_COUNT   4
#define SYMBOL_COUNT  2

#define COLOR_OUTPUT  1
#if COLOR_OUTPUT == 1
  #define COLOR_RED     "\x1b[31m"
  #define COLOR_GREEN   "\x1b[32m"
  #define COLOR_YELLOW  "\x1b[33m"
  #define COLOR_BLUE    "\x1b[34m"
  #define COLOR_MAGENTA "\x1b[35m"
  #define COLOR_CYAN    "\x1b[36m"
  #define COLOR_RESET   "\x1b[0m"
#else
  #define COLOR_RED     ""
  #define COLOR_GREEN   ""
  #define COLOR_YELLOW  ""
  #define COLOR_BLUE    ""
  #define COLOR_MAGENTA ""
  #define COLOR_CYAN    ""
  #define COLOR_RESET   ""
#endif

typedef struct {
  int table[STATE_COUNT][SYMBOL_COUNT];
  int state;
  int halt;
} DFA;

typedef struct {
  char alphabet[SYMBOL_COUNT];
  int size;
} Language;

static const Language language = {ALPHABET, SYMBOL_COUNT};

int getTransitionCode(char symbol) {
  for (int i = 0; i < language.size; ++i) {
    if (symbol == language.alphabet[i]) {
      return i;
    }
  }
  return -1;
}

int regExprMatch(char string[]) {
  DFA  automata = {TABLE, START_STATE, HALT_STATE};
  int  counter  = 0;
  char current  = string[0];
  while (current != '\0') {
    int transition = getTransitionCode(current);
    if (transition == -1) { return EXIT_FAILURE; }
    automata.state = automata.table[automata.state][transition];
    ++counter;
    current = string[counter];
  }
  if (automata.state == automata.halt) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
  /*
   * Get some input file. For each whitespace delimited string, check against
   * all DFAs according to DFA matching rules.
   */
  for (int i = 1; i < argc; ++i) {
    if (regExprMatch(argv[i]) == EXIT_FAILURE) {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
