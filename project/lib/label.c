#include <stdio.h>
#include <string.h>
#include <math.h>

int label_count = 0;

char labels[7][6] = {
    "wumbo",
    "fargo",
    "green",
    "happy",
    "chirp",
    "boots",
    "splat"
};

char *get_label() {
    char *str = (char *) malloc(6 * sizeof(char));
    str = labels[label_count];
    ++label_count;
    return str;
}
