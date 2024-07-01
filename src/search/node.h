#pragma once

#include "chess/move.h"

typedef struct Node {
    struct Node *children;
    struct Node *parent;
    Move move;
    int children_count;
    uint64_t visits;
    double standing;
} Node;

Node GenerateNode(Node *parent, Move move);
int NodeDepth(Node *node);
double NodeScore(Node *node);
void NodeClean(Node *node);
Node *BestChild(Node *node);
int TreeSize(Node *node);
