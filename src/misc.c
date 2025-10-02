#include <time.h>

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

void init(void) {
	init_masks();
	init_zobrist();
	init_values();
	init_tt();
}

double time_ns() {
        struct timespec t;
	timespec_get(&t, TIME_UTC);
	return (double) t.tv_sec * 1e9 + t.tv_nsec;
}
