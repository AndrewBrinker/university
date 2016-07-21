#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>


/**
 * Get the length of the given string.
 * @param  s -> The string
 * @return the length
 */
unsigned int xstrlen(const char *restrict s) {
    unsigned int n;
    for (n = 0; *(s + n); ++n);
    return n;
}


/**
 * Copy s into t. This assumes that t is large enough to hold s, and that s is
 * null terminated.
 * @param t -> The target
 * @param s -> The source
 */
void xstrcpy(char *restrict t, const char *restrict s) {
    for (; (*t++ = *s++););
}


/**
 * Copy at most n characters from s into t. This assumes that t has a size of at
 * least n, and that s (if shorter than n) is null terminated.
 * @param t -> The target
 * @param s -> The source
 * @param n -> The maximum number of characters to copy
 */
void xstrncpy(char *restrict t, const char *restrict s, const size_t n) {
    unsigned int i;
    for (i = 0; i < n && *(s + i); ++i) {
        *(t + i) = *(s + i);
    }
    *(t + i) = '\0';
}


/**
 * Concatenate s into the end of t. This assumes that t can hold s, and that s
 * is null terminated.
 * @param t -> The target
 * @param s -> The source
 */
void xstrcat(char *restrict t, const char *restrict s) {
    unsigned int i;
    for (i = 0; *(t + i); ++i);
    xstrcpy(t + i, s);
}


/**
 * Check whether t and s are identical, and return an integer value indicating
 * the degree of difference. A greater absolute value of the return indicates
 * a greater degree of difference.
 * @param  t -> The first string
 * @param  s -> The second string
 * @return an integer indicating the degree of difference.
 */
int xstrcmp(const char *restrict t, const char *restrict s) {
    for (; *t && (*t == *s); ++t, ++s);
    return *(const unsigned char *) t - *(const unsigned char *) s;
}


int main() {
    char str1[20] = "abcd";
    char str2[20] = "efg";

    printf("str1                = %s\n", str1);
    printf("str2                = %s\n", str2);

    printf("strlen(str1)        = %d\n", xstrlen(str1));
    printf("strlen(str2)        = %d\n", xstrlen(str2));

    xstrcpy(str1, str2);
    printf("str1                = %s\n", str1);

    xstrcat(str1, str2);
    printf("str2                = %s\n", str2);

    printf("strcmp(str1, str2)  = %d\n", xstrcmp(str1, str2));
    printf("strcmp(str2, 'efh') = %d\n", xstrcmp(str2, "efh"));

    xstrncpy(str2, "hi jkl", 2);
    printf("str2                = %s\n", str2);

    xstrncpy(str2, "hi jkl", 12);
    printf("str2                = %s\n", str2);

    return 0;
}
