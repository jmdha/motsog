#include "node.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

static double EXPLORATION_MODIFIER = 0.1f;

Node GenerateNode(Node *parent, Move move) {
    assert(parent == 0 || move != 0);
    Node node;
    node.parent = parent;
    node.move = move;
    node.visits = 0;
    node.standing = 0;
    node.children_count = 0;
    return node;
}

int NodeDepth(Node *node) {
    int i = 0;
    Node *parent = node->parent;
    while (parent != 0) {
        i++;
        parent = parent->parent;
    }
    return i;
}

double NodeScore(Node *node) {
    if (node->visits == 0)
        return 100000;

    Node *parent = node;
    if (node->parent != 0)
        parent = node->parent;

    const double exploitation_score = node->standing / node->visits;
    const double exploration_score = sqrt(log(parent->visits) / node->visits);

    return exploitation_score + EXPLORATION_MODIFIER * exploration_score;
}

void NodeClean(Node *node) {
    if (node->children_count > 0) {
        for (int i = 0; i < node->children_count; i++)
            NodeClean(&node->children[i]);
        free(node->children);
    }
}

Node *BestChild(Node *node) {
    if (node->children_count == 0)
        return 0;
    uint64_t highest_rating = node->children[0].visits;
    int best = 0;
    for (int i = 1; i < node->children_count; i++)
        if (node->children[i].visits > highest_rating) {
            highest_rating = node->children[i].visits;
            best = i;
        }
    return &node->children[best];
}

int TreeSize(Node *node) {
    int s = 0;
    for (int i = 0; i < node->children_count; i++)
        s += TreeSize(&node->children[i]);
    return s + 1;
}
