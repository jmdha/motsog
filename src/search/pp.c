#include <assert.h>
#include <limits.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "pp.h"
#include "chess/move.h"

#define PP_MAX 1

typedef struct Node {
    unsigned int count;
    int *evals;
    Move *moves;
    struct Node *children;
    unsigned int ply;
} Node;

Node *parents[256];
Node *node = NULL;
unsigned int ply = 0;

void PPInit() { node = calloc(1, sizeof(Node)); }

void PPEnter(Move move) {
    if (ply == node->ply && ply < PP_MAX) {
        unsigned int index = INT_MAX;
        for (unsigned int i = 0; i < node->count; i++) {
            if (node->moves[i] == move) {
                index = i;
                break;
            }
        }
        if (index == INT_MAX) {
            index = node->count;
            node->count++;
            node->evals = realloc(node->evals, node->count * sizeof(int));
            node->moves = realloc(node->moves, node->count * sizeof(Move));
            node->children = realloc(node->children, node->count * sizeof(Node));
            node->evals[index] = 0;
            node->moves[index] = move;
            memset(&node->children[index], 0, sizeof(Node));
            node->children[index].ply = node->ply + 1;
        }
        parents[ply] = node;
        node = &node->children[index];
    }
    ply++;
}

void PPExit() {
    if (ply == node->ply) {
        node = parents[ply - 1];
    }
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
    if (index != INT_MAX)
        node->evals[index] = eval;
}

PPResult PPRetrieve() {
    PPResult result = {.count = 0};

    if (ply == node->ply) {
        result.count = node->count;
        result.moves = node->moves;
        result.evals = node->evals;
    }

    return result;
}

Move PPBestMove() {
    unsigned int best_index = INT_MAX;
    int best_eval = -INT_MAX;

    for (unsigned int i = 0; i < node->count; i++)
        if (node->evals[i] > best_eval || best_index == INT_MAX) {
            best_index = i;
            best_eval = node->evals[i];
        }

    return node->moves[best_index];
}

void PPPrintPV() {
    Node *current = node;
    while (current) {
        unsigned int best_index = INT_MAX;
        int best_eval = -INT_MAX;

        for (unsigned int i = 0; i < current->count; i++)
            if (current->evals[i] > best_eval || best_index == INT_MAX) {
                best_index = i;
                best_eval = current->evals[i];
            }

        if (best_index == INT_MAX)
            break;
        printf(" ");
        PrintMove(current->moves[best_index]);
        current = &current->children[best_index];
    }
}
