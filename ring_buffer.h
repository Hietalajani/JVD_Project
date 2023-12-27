#ifndef JVD_PROJECT_RING_BUFFER_H
#define JVD_PROJECT_RING_BUFFER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// struct declaration
typedef struct  {
    int head;
    int tail;
    int size;
    uint8_t *buffer;
} ring_buffer;

// declare functions
void initialize_rb(ring_buffer *rb, int size);
bool rb_empty(ring_buffer *rb);
bool rb_full(ring_buffer *rb);
int rb_read(ring_buffer *rb);
bool rb_write(ring_buffer *rb, uint8_t input);

#endif //JVD_PROJECT_RING_BUFFER_H
