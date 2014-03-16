

void chomp(char *str, size_t n) {
    if (str[n-1] == '\n') str[n-1] == '\0';
}