#ifndef UTILC_ITERATOR_H
#define UTILC_ITERATOR_H


typedef struct IntIterator {
    char impl[32]; // impl max size = 32 bytes
    int *end;
    const int *(*next)(struct IntIterator *self);
} IntIterator;


struct IntIterator_range_s_ {
    int i;
    int step;
    int end;
};

static const int *IntIterator_range_next_(IntIterator *self) {
    struct IntIterator_range_s_ *impl = (struct IntIterator_range_s_ *) self->impl;
    impl->i+=impl->step;
    return impl->i < impl->end ? &impl->i : NULL;
}

static const int *IntIterator_range_reverse_next_(IntIterator *self) {
    struct IntIterator_range_s_ *impl = (struct IntIterator_range_s_ *) self->impl;
    impl->i+=impl->step;
    return impl->i > impl->end ? &impl->i : NULL;
}

/**
 * Creates an IntIterator from a range of [start:end) : step
 * @returns the pointer to the first value
 */
static const int *IntIterator_full_range_begin(IntIterator *self, int start, int end, int step) {
    struct IntIterator_range_s_ *impl = (struct IntIterator_range_s_ *) self->impl;
    impl->i = start;
    impl->step = step;
    impl->end = end;
    self->end = NULL;
    if(impl->step >=0) {
        self->next = &IntIterator_range_next_;
        return impl->i < impl->end ? &impl->i : NULL;
    }
    self->next = &IntIterator_range_reverse_next_;
    return impl->i > impl->end ? &impl->i : NULL;
}

/**
 * Creates an IntIterator from a range of [0:end) : 1
 * @returns the pointer to the first value
 */
static const int *IntIterator_range_begin(IntIterator *self, int end) {
    return IntIterator_full_range_begin(self, 0, end, 1);
}





static const int *IntIterator_indices_next_(IntIterator *self) {
    int **index = (int **) self->impl;
    (*index)++;
    return *index;
}

/**
 * Creates an IntIterator from indices
 * @returns the pointer to the first value
 */
static const int *IntIterator_indices_begin(IntIterator *self, int *indices, int size) {
    int **index = (int **) self->impl;
    *index = indices;
    self->end = indices + size;
    self->next = &IntIterator_indices_next_;
    return *index;
}

#endif //UTILC_ITERATOR_H
