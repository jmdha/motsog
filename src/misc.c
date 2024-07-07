#include "misc.h"
#include "chess/attacks.h"
#include "chess/position.h"

void Init(void) {
    InitAttacks();
    InitZobrist();
}

void Fini(void) {

}
