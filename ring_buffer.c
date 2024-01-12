#include "ring_buffer.h"

void initialize_rb(ring_buffer *rb, int size) {
    uint8_t *buffer = calloc(size, sizeof(uint8_t));

    rb->tail = 0;
    rb->head = 0;
    rb->size = size;
    rb->buffer = buffer;
}

bool rb_empty(ring_buffer *rb) {
    return rb->head == rb->tail;
}

bool rb_full(ring_buffer *rb) {
    return (rb->head + 1) % rb->size == rb->tail;
}

int rb_read(ring_buffer *rb) {
    uint8_t output = rb->buffer[rb->tail];
    if (rb->head != rb->tail) rb->tail = (rb->tail + 1) % rb->size;
    return output;
}

bool rb_write(ring_buffer *rb, uint8_t input) {
    int newhead = (rb->head + 1) % rb->size;
    if (newhead == rb->tail) return false;

    rb->buffer[rb->head] = input;
    rb->head = newhead;
    return true;
}

void rb_free(ring_buffer *rb) {
    free(rb->buffer);
}


