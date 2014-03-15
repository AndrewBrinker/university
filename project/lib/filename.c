#include <stdlib.h>
#include <string.h>

#define DOT '.'
#define SEP '/'
#define EXT ".cpp"


/**
 * Strip the file extension from the given file name.
 * @param  input -> The file name being stripped
 * @return the stripped file name
 */
char *strip_extension(const char *input) {
    char *stripped;
    char *last_dot;
    char *last_sep;
    if (input == NULL) return NULL;
    if ((stripped = (char *) malloc(strlen(input) + 1)) == NULL) return NULL;
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
    return stripped;
}


/**
 * Convert file name from "blah.b" to "blah.c"
 *
 * Strips the current file extension from the file name, and replaces it with
 * the new extension. Requires that the created string be manually deallocated.
 *
 * @param  input -> The current file name
 * @return the ".c" extension file name
 */
char *convert_file_name(const char* input) {
    char *stripped = strip_extension(input);
    size_t length = strlen(stripped) + strlen(EXT);
    char *expanded = (char*) malloc(length * sizeof(char) + 1);
    *expanded = '\0';
    expanded = strcat(stripped, EXT);
    return expanded;
}
