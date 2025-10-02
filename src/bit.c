#include <assert.h>

#include "bit.h"

int popcount(uint64_t v) { 
	return __builtin_popcountll(v); 
}

int lsb(uint64_t v) {
	assert(v);
	return __builtin_ctzll(v);
}

int msb(uint64_t v) {
	assert(v);
	return __builtin_clzll(v) ^ 63;
}

int lsbpop(uint64_t *v) {
	int b = lsb(*v);
	*v &= *v - 1;
	return b;
}

int msbpop(uint64_t *v) {
	int b = msb(*v);
	*v ^= 1ull << b;
	return b;
}

bool multiple(uint64_t v) {
	return (v & (v - 1)) != 0;
}
