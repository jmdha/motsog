#include "misc.h"
#include "chess/masks.h"
#include "chess/position.h"
#include "eval/values.h"

void memswap(void *restrict buffer, void *l, void *r, size_t size) {
    memcpy(buffer, l, size);
    memmove(l, r, size);
    memcpy(r, buffer, size);
}

void Init(void) {
    InitMasks();
    init_zobrist();
    ValueInit();
}

void Fini(void) {}
