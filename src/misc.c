#include "misc.h"
#include "chess/masks.h"
#include "chess/zobrist.h"
#include "eval/values.h"
#include "search/tt.h"

void memswap(void *restrict buffer, void *l, void *r, size_t size) {
    memcpy(buffer, l, size);
    memmove(l, r, size);
    memcpy(r, buffer, size);
}

#ifdef WIN32
#include <windows.h>
uint64_t time_ms() {
    return GetTickCount();
}
#else
#include <sys/time.h>
uint64_t time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
#endif

void init(void) {
    init_masks();
    init_zobrist();
    init_values();
    init_tt();
}

void fini(void) {
    fini_tt();
}
