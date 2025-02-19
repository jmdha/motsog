#pragma once

#include <memory.h>
#include <stdint.h>

void memswap(void *restrict buffer, void *l, void *r, size_t size);

#define swap(a, b)                                                                                 \
    memswap(&(struct {                                                                             \
        _Static_assert(sizeof *(a) == sizeof *(b), "arguments of swap must have same size");       \
        char _[sizeof *(a)];                                                                       \
    }){0},                                                                                         \
            (a), (b), sizeof *(a))

#define max(a, b)                                                                                  \
    ({                                                                                             \
        __typeof__(a) _a = (a);                                                                    \
        __typeof__(b) _b = (b);                                                                    \
        _a > _b ? _a : _b;                                                                         \
    })

#define min(a, b)                                                                                  \
    ({                                                                                             \
        __typeof__(a) _a = (a);                                                                    \
        __typeof__(b) _b = (b);                                                                    \
        _a < _b ? _a : _b;                                                                         \
    })

uint64_t time_ms();

void init(void);
void fini(void);
