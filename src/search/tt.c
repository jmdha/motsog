#include <memory.h>
#include <stdlib.h>
#include <limits.h>

#include "tt.h"


typedef uint64_t tt_entry;

unsigned int COUNT   = 0;
tt_entry*    TT      = NULL;

void init_tt() {
	const int b  = 1 * 1024 * 1024;
	COUNT        = b / sizeof(tt_entry);
	TT           = calloc(COUNT, sizeof(tt_entry));
}

void fini_tt() {
	free(TT);
}

int tt_hash_full() {
	unsigned int full = 0;
	
	for (unsigned int i = 0; i < COUNT; i++)
		if (TT[i] != 0)
			full++;
	
	return full * 1000 / COUNT;
}

void tt_clear() {
	for (unsigned int i = 0; i < COUNT; i++)
		TT[i] = 0;
}

Move tt_probe(Hash hash) {
	tt_entry e = TT[hash % COUNT];
	if ((hash & 0xffffffffffff0000) == (e & 0xffffffffffff0000))
		return e & 0xffff;
	else
		return 0;
}

void tt_store(Hash hash, Move move) {
	tt_entry* e = &TT[hash % COUNT];
	*e  = move;
	*e |= hash & 0xffffffffffff0000;
}
