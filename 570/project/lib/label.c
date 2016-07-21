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

char labels[32][6] = {
    "aaaaa", "aaaab", "aaaba", "aaabb", "aabaa", "aabab", "aabba", "aabbb",
    "abaaa", "abaab", "ababa", "ababb", "abbaa", "abbab", "abbba", "abbbb",
    "baaaa", "baaab", "baaba", "baabb", "babaa", "babab", "babba", "babbb",
    "bbaaa", "bbaab", "bbaba", "bbabb", "bbbaa", "bbbab", "bbbba", "bbbbb",
};


/**
 * Add a new label to the list of labels.
 * @param  identifier -> The identifier being used
 * @return a pointer to the added label
 */
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


/**
 * Get the label associated with the given integer.
 * @param  identifier -> The integer identifier
 * @return a pointer to the label, or NULL
 */
label *getlabel(const int identifier) {
    label *ptr;
    for (ptr = label_list;
        ptr != NULL;
        ptr  = ptr->next) {
        if (ptr->id == identifier) {
            return ptr;
        }
    }
    return NULL;
}
