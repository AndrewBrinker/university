#include <string.h>

void chomp(char *str, size_t n) {
    if (str[n-1] == '\n') str[n-1] = '\0';
}

char *string_combine(const char *str1, const char *str2, const char *str3) {
    size_t size = strlen(str1) + strlen(str2) + strlen(str3) + 1;
    char *new_string = malloc(size * sizeof(char));
    size_t i = 0;
    for (; i < strlen(str1); ++i) new_string[i] = str1[i];
    for (; i < strlen(str2); ++i) new_string[i] = str2[i];
    for (; i < strlen(str3); ++i) new_string[i] = str3[i];
    new_string[size - 1] = '\0';
    return new_string;
}