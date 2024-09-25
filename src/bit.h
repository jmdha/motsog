#pragma once

#include <stdbool.h>
#include <stdint.h>

int popcount(uint64_t v);  // Returns the number of bits set
int lsb(uint64_t v);       // Returns the index of the smallest bit set
int msb(uint64_t v);       // Returns the index of the biggest bit set
int lsbpop(uint64_t *v);   // Returns and removes the index of the smallest bit set
int msbpop(uint64_t *v);   // Returns and removes the index of the biggest bit set
bool multiple(uint64_t v); // Returns whether multiple bits are set
