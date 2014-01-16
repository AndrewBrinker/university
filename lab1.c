/*
 * Author:    Andrew Brinker
 * Course:    CSE 570, Compilers
 * Professor: Dr. Ernesto Gomez
 * Term:      Winter, 2013-2014
 * License:   All rights reserved (don't copy this for your own homework, man)
 *
 * Defines a table-based deterministic finite-state automata used for testing
 * the given string against the following regular expression:
 *
 *     (a|b)*abb
 *
 * This program should be trivially adaptable to different regular expressions.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/*
 * All the configuration is here. Just change these macros to fit the regular
 * expression you are trying to match strings against, and everything will
 * adjust automatically. Ah, the wonders of programming.
 */

#define EXPRESSION    "(a|b)*abb"
#define TABLE         {{1,0}, {0,2}, {0, 3}, {0, 0}}
#define ALPHABET      {'a', 'b'}
#define STATE_COUNT   4
#define SYMBOL_COUNT  2
#define START_STATE   0
#define HALT_STATE    3


/*
 * Some nice terminal coloring using ANSI color codes. You can turn it off if
 * it's giving you trouble by just setting COLOUR_OUTPUT to anything besides 1.
 */

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


/*
 * A couple of useful structs. Each is only used once, but collecting these
 * values together logically makes for cleaner code down the road.
 */

typedef struct {
  int table[STATE_COUNT][SYMBOL_COUNT];
  int state;
  int halt;
} DFA;

typedef struct {
  char alphabet[SYMBOL_COUNT];
  int size;
} Language;


/*
 * Avoids constant reinitialization of the struct each time getTransition() is
 * called. Not a huge performance gain, but it's something.
 */

static Language language = {ALPHABET, SYMBOL_COUNT};


/*
 * Tests whether the given symbol is in the given language.
 * Returns a transition value equal to the symbol's position in the alphabet
 * if present. Returns -1 otherwise.
 */

int getTransitionCode(char symbol) {
  for (int i = 0; i < language.size; ++i) {
    if (symbol == language.alphabet[i]) {
      return i;
    }
  }
  return -1;
}


/*
 * Initializes a deterministic finite state automata (try saying that 10 times
 * fast), and then uses it to transition through the appropriate state
 * transitions based on the input. Tests whether the inputted string matches
 * the previously defined regular expression.
 */

int regularExpressionMatch(char string[]) {
  DFA  automata = {TABLE, START_STATE, HALT_STATE};
  int  counter  = 0;
  char current  = string[0];
  while (current != '\0') {
    int transition = getTransitionCode(current);
    if (transition == -1) { return EXIT_FAILURE; }
    automata.state = automata.table[automata.state][transition];
    current = string[counter];
    ++counter;
  }
  if (automata.state == automata.halt) {
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
}


/*
 * The lexing interface. All the printing happens here. May opt to move this to
 * another file later, but for now this works.
 *
 * It has some coloring to make the output look nice. It's not really needed,
 * but it doesn't really hurt anything. At least for now this file doesn't have
 * to interact this anything else, so I'm not super concerned about output being
 * machine-readable.
 */

int main(int argc, char *argv[]) {
  printf("\n");
  if (argc < 2) {
    printf("Please input at least 1 string to test.\n");
    return 0;
  }
  printf(COLOR_CYAN "Matching input against: %s\n\n" COLOR_RESET, EXPRESSION);
  for (int i = 1; i < argc; ++i) {
    printf("Input String: " COLOR_MAGENTA "%s\n" COLOR_RESET, argv[i]);
    switch (regularExpressionMatch(argv[i])) {
      case EXIT_SUCCESS:
        printf(COLOR_GREEN "  > Input accepted!\n\n" COLOR_RESET);
        break;
      case EXIT_FAILURE:
        printf(COLOR_RED "  ! Input failed.\n\n" COLOR_RESET);
        break;
    }
  }
  printf("\n");
  return 0;
}
