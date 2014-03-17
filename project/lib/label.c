#include <stdio.h>
#include <string.h>
#include <math.h>

int label_count = 0;

typedef struct label {
    int id;
    char *name;
    struct label *next;
} label;

label *label_list = 0;

char labels[7][6] = {
    "wumbo",
    "fargo",
    "green",
    "happy",
    "chirp",
    "boots",
    "splat"
};

label *putlabel(const int identifier) {
    label *new_label = (label *) malloc(sizeof(label));
    if (!new_label) return 0;
    new_label->name = labels[label_count];
    new_label->id   = identifier;
    new_label->next = label_list;
    label_list = new_label;
    ++label_count;
    return new_label;
}

label *getlabel(const int identifier) {
    label *ptr;
    for (ptr = label_list;
        ptr != 0;
        ptr  = ptr->next) {
        if (ptr->id == identifier) {
            return ptr;
        }
    }
    return 0;
}
