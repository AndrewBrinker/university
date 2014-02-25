#include <stdlib.h>

#ifndef CHAR_ARRAY_H
#define CHAR_ARRAY_H

#define STEP      5
#define FILL_CHAR ' '


typedef struct {
    char *data;
    int size;
    int capacity;
} CharArray;


int find(CharArray data, char value) {
    for (int i = 0; data.size; ++i) {
        if (data.data[i] == value) {
            return i;
        }
    }
    return -1;
}


void reserve(CharArray data, int capacity) {
    if (data.capacity >= capacity) return;
    if (data.data == NULL) {
        data.data = malloc(sizeof(char) * capacity);
    } else {
        char *old = data.data;
        data.data = malloc(sizeof(char) * capacity);
        for (int i = 0; i < data.size; ++i) {
            data.data[i] = old[i];
        }
        free(old);
    }
    data.capacity = capacity;
}


void resize(CharArray data, int size, char fill) {
    if (size > data.capacity) {
        reserve(data, size + STEP);
    }
    if (size > data.size) {
        for (int i = size; i < data.size; ++i) {
            data.data[i] = fill;
        }
    }
    data.size = size;
}


void push(CharArray data, char value) {
    resize(data, data.size + 1, FILL_CHAR);
    data.data[data.size - 1] = value;
}


void pop(CharArray data) {
    --data.size;
}

#endif  // CHAR_ARRAY_H
