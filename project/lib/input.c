#include <stdio.h>
#include <string.h>

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2


/**
 * Get a line of input from the user.
 * @param  prompt -> The text displayed to the user
 * @param  buffer -> The input received from the user.
 * @param  size   -> The size of the buffer
 * @return OK, NO_INPUT, or TOO_LONG to indicate status
 */
int get_line(const char *prompt, char *buffer, size_t size) {
    int character;
    int extra;
    if (prompt != NULL) {
        printf("%s", prompt);
        fflush(stdout);
    }
    if (fgets(buffer, size, stdin) == NULL) {
        return NO_INPUT;
    }
    if (buffer[strlen(buffer) - 1] != '\n') {
        extra = 0;
        while (((character = getchar()) != '\n') && (character != EOF)) {
            extra = 1;
        }
        return (extra == 1) ? TOO_LONG : OK;
    }
    buffer[strlen(buffer) - 1] = '\0';
    return OK;
}
