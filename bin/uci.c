#include "chess/attacks.h"
#include "chess/board.h"
#include "chess/perft.h"
#include "chess/zobrist.h"
#include "search/mcts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool GetLine(char *str) {
    char *ptr;
    if (fgets(str, 8192, stdin) == NULL)
        return 0;

    ptr = strchr(str, '\n');
    if (ptr != NULL)
        *ptr = '\0';

    ptr = strchr(str, '\r');
    if (ptr != NULL)
        *ptr = '\0';

    return true;
}

void UCIPosition(Board *board, char *buf) {
    if (strstr(buf, "startpos") != NULL)
        *board = DefaultBoard();
    else if (strstr(buf, "fen") != NULL)
        *board = ImportFEN(strstr(buf, "fen") + strlen("fen "));

    if (strstr(buf, "moves") != NULL)
        ImportMoves(board, strstr(buf, "moves") + strlen("moves "));
}

void UCIGo(Board *board, char *buf) {
    uint64_t time = ~0; // If no time specified, go infinite (or atleast a very long time)
    for (char *p = strtok(buf, " "); p != NULL; p = strtok(NULL, " ")) {
        if ((GetPosition(board)->turn == WHITE && strcmp(p, "wtime") == 0) ||
            (GetPosition(board)->turn == BLACK && strcmp(p, "btime") == 0)) {
            time = atoi(strtok(NULL, " "));
            break;
        }
    }
    Move move = FindBestMove(board, 0.05 * time);
    printf("bestmove ");
    PrintMove(move);
    printf("\n");
    fflush(stdout);
}

// TODO: Allow stop command (Would require multiple threads)
int main(int argc, char **argv) {
    InitZobrist();
    InitAttacks();

    Board board = DefaultBoard();
    if (argc > 1) {
        if (strcmp(argv[1], "go") == 0)
            UCIGo(&board, argv[1]);
    }

    char buf[8192] = {0};
    setbuf(stdout, NULL);

    while (GetLine(buf)) {
        if (strcmp(buf, "uci") == 0) {
            printf("id name %s %s\n", _PROJECT_NAME, _PROJECT_VERSION);
            printf("id author %s\n", _PROJECT_AUTHOR);
            printf("uciok\n"), fflush(stdout);
        } else if (strcmp(buf, "isready") == 0)
            printf("readyok\n"), fflush(stdout);
        else if (strcmp(buf, "ucinewgame") == 0)
            board = DefaultBoard();
        else if (strcmp(buf, "quit") == 0)
            break;
        else if (strstr(buf, "perft") == buf)
            printf("%lu\n", Perft(&board, atoi(buf + strlen("perft ")))), fflush(stdout);
        else if (strstr(buf, "position") == buf)
            UCIPosition(&board, buf);
        else if (strstr(buf, "go") == buf)
            UCIGo(&board, buf);
    }

    return 0;
}
