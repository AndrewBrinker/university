#include <stdlib.h>
#include <string.h>

#define DOT '.'
#define SEP '/'
#define EXT ".c"

/**
 * Convert file name from "blah.tbsc" to "blah.c"
 *
 * Strips the current file extension from the file name, and replaces it with the
 * new extension. Requires that the created string be manually deallocated.
 *
 * @param  input -> The current file name
 * @return the ".c" extension file name
 */
char *convert_file_name(const char* input) {
    char *stripped;
    char *last_dot;
    char *last_sep;
    if (input == NULL) return NULL;
    if ((stripped = malloc(strlen(input) + 1)) == NULL) return NULL;
    strcpy(stripped, input);
    last_dot = strrchr(stripped, DOT);
    last_sep = (SEP == 0) ? NULL : strrchr(stripped, SEP);
    if (last_dot != NULL) {
        if (last_sep != NULL) {
            if (last_sep < last_dot) {
                *last_dot = '\0';
            }
        } else {
            *last_dot = '\0';
        }
    }
    size_t length = strlen(input) + strlen(EXT);
    char *ret = (char*) malloc(length * sizeof(char) + 1);
    *ret = '\0';
    char *combined = strcat(strcat(ret, input), EXT);
    free(stripped);
    return combined;
}
