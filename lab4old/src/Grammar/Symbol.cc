#include "Symbol.h"

Symbol::Symbol(char new_name, bool terminal = false) {
    name = new_name;
    is_terminal = terminal;
}