#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#include "pp.h"

typedef struct Node {
    unsigned int count;
    int *evals;
    Move *moves;
    struct Node *children;
    unsigned int ply;
} Node;

Node *parent = NULL;
Node *node = NULL;
unsigned int ply = 0;

void PPInit() {
    node = calloc(1, sizeof(Node));
}

void PPEnter(Move move) {
    ply++;
}

void PPExit() {
    ply--;
}

void PPStore(Move move, int eval) {
    if (ply != node->ply) 
        return;
    unsigned int index = INT_MAX;
    for (unsigned int i = 0; i < node->count; i++) {
        if (node->moves[i] == move) {
            index = i;
            break;
        }
    }
    if (index == INT_MAX){
        node->count++;
        node->evals = realloc(node->evals, node->count * sizeof(int));
        node->moves = realloc(node->moves, node->count * sizeof(Move));
        node->children = realloc(node->children, node->count * sizeof(Node));
        node->moves[node->count - 1] = move;
        node->evals[node->count - 1] = eval;
    } else {
        node->evals[index] = eval;
    }
}

Move PPRetrieve() {
    unsigned int best_index = INT_MAX;
    int best_eval = -INT_MAX;
   
    for (unsigned int i = 0; i < node->count; i++)
        if (node->evals[i] > best_eval) {
            best_index = i;
            best_eval = node->evals[i];
        }

    return node->moves[best_index];
}
