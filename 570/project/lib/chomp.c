#include <string.h>

/**
 * Remove any trailing newline from string
 * @param str -> The string being modified
 * @param n   -> The size of the string
 */
void chomp(char *str) {
    size_t n = strlen(str);
    if (str[n - 1] == '\n') {
        str[n - 1] = '\0';
    }
}
