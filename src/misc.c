#include "misc.h"
#include "chess/attacks.h"
#include "chess/position.h"
#include "values.h"

void memswap(void *restrict buffer, void *l, void *r, size_t size) {
    memcpy(buffer, l, size);
    memmove(l, r, size);
    memcpy(r, buffer, size);
}

void Init(void) {
    InitAttacks();
    InitZobrist();
    ValueInit();
}

void Fini(void) {

}
