#pragma once

#include <memory.h>

void memswap(void *restrict buffer, void *l, void *r, size_t size);

#define swap(a, b)                                                                                 \
    memswap(&(struct {                                                                             \
        _Static_assert(sizeof *(a) == sizeof *(b), "arguments of swap must have same size");       \
        char _[sizeof *(a)];                                                                       \
    }){0},                                                                                         \
            (a), (b), sizeof *(a))

void Init(void);
void Fini(void);
