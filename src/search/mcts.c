#include "mcts.h"
#include "chess/board.h"
#include "chess/move_gen.h"
#include "chess/position.h"
#include "node.h"
#include <assert.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

typedef enum { WIN = 1, LOSS = -1, DRAW = 0 } Result;

size_t simulations_lengths[1000000];
size_t simulations = 0;

Node *Selection(Board *board, Node *root) {
    Node *node = root;

    while (node->visits > 0 && node->children_count > 0) {
        float max_score = NodeScore(&node->children[0]);
        int max_child = 0;
        for (int i = 1; i < node->children_count; i++) {
            const float score = NodeScore(&node->children[i]);
            if (score > max_score) {
                max_child = i;
                max_score = score;
            }
        }
        node = &node->children[max_child];
        ApplyMove(board, node->move);
    }

    return node;
}

Node *Expansion(Board *board, Node *node) {
    if (NodeDepth(node) > 3) return node;
    Move legal_moves[MAX_MOVES];
    node->children_count = GenerateLegalMoves(board, legal_moves);

    if (node->children_count > 0) {
        node->children = malloc(node->children_count * sizeof(Node));
        for (int i = 0; i < node->children_count; i++)
            node->children[i] = GenerateNode(node, legal_moves[i]);
        node = &node->children[RandomNumber() % node->children_count];
        ApplyMove(board, node->move);
    }

    return node;
}

Result Simulation(Board *board) {
    const Color seed_turn = GetPosition(board)->turn;
    Move moves[MAX_MOVES];

    Result result;

    size_t len = 0;
    while (true) {
        if (IsThreefold(board)) {
            result = DRAW;
            break;
        }
        Position *pos = GetPosition(board);
        const int move_count = GenerateMoves(pos, moves);

        bool found = false;
        uint64_t rnd = (move_count != 0) ? RandomNumber() % move_count : 0;
        for (int i = 0; i < move_count; i++) {
            Move move = moves[(rnd + i) % move_count];
            ApplyMove(board, move);
            if (IsKingSafe(GetPosition(board), !GetPosition(board)->turn)) {
                found = true;
                break;
            }
            UndoMove(board, move);
        }

        if (!found) {
            if (GenerateAttackBoard(pos, !pos->turn) & pos->pieces[KING])
                result = (pos->turn == seed_turn) ? WIN : LOSS;
            else
                result = DRAW;
            break;
        }
        len++;
    }

    simulations_lengths[simulations++] = len;

    return result;
}

void BackPropagation(Node *node, Result result) {
    while (node != 0) {
        node->visits++;
        node->standing += result;
        node = node->parent;
        result *= -1;
    }
}

Move FindBestMove(Board *board, uint64_t time_limit) {
    simulations = 0;
    const int root_depth = board->move_depth;
    Node root = GenerateNode(0, 0);
    clock_t start = clock();
    uint64_t t = 0;
    uint64_t next_print = 100;
    do {
        Node *leaf = Selection(board, &root);
        assert(leaf != 0);
        leaf = Expansion(board, leaf);
        assert(leaf != 0);
        Result result = Simulation(board);
        // Undoes all moves done in simulation
        board->move_depth = root_depth;
        BackPropagation(leaf, result);

        if (root.visits % 100 == 0)
            t = (float)(clock() - start) / CLOCKS_PER_SEC * 1000;
        if (root.visits == next_print) {
            printf("info");
            printf(" simulations %lu", root.visits);
            printf(" sps %lu", root.visits * 1000 / (t > 0 ? t : 1));
            printf(" nodes %lu", board->moves);
            printf(" nps %lu", board->moves * 1000 / (t > 0 ? t : 1));
            printf(" time %lu", t);
            Node *best_child = BestChild(&root);
            if (best_child != 0) {
                printf(" score cp %f", NodeScore(best_child));
                printf(" pv ");
                PrintMove(best_child->move);
            }
            printf("\n");
            next_print *= 10;
        }
    } while (t < time_limit);
    printf("info tree size %d\n", TreeSize(&root));
    size_t total = 0;
    for (size_t i = 0; i < simulations; i++)
        total += simulations_lengths[i];
    size_t avg = total / simulations;
    printf("info avg simulation length %zu\n", avg);
    Move best_move = BestChild(&root)->move;
    NodeClean(&root);
    return best_move;
}
