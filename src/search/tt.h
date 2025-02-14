#pragma once

#include "chess/types.h"
#include <stdbool.h>

void init_tt();
void fini_tt();

int  tt_hash_full();
void tt_clear();
Move tt_probe(Hash hash);
void tt_store(Hash hash, Move move);
