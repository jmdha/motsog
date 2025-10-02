#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "chess/move.h"
#include "chess/position.h"
#include "misc.h"
#include "search/search.h"
#include "search/tt.h"

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

Position UCIPosition(char *buf) {
    Position pos;
    if (strstr(buf, "startpos") != NULL)
        pos = position();
    else if (strstr(buf, "fen") != NULL)
        pos = import(strstr(buf, "fen") + strlen("fen "));
    else
        abort();

    if (strstr(buf, "moves") != NULL)
        apply_moves(&pos, strstr(buf, "moves") + strlen("moves "));
    return pos;
}

void UCIGo(Position *pos, char *buf) {
    uint64_t time = ~0; // If no time specified, go infinite (or atleast a very long time)
    for (char *p = strtok(buf, " "); p != NULL; p = strtok(NULL, " ")) {
        if ((pos->turn == WHITE && strcmp(p, "wtime") == 0) ||
            (pos->turn == BLACK && strcmp(p, "btime") == 0) || strcmp(p, "movetime") == 0) {
            time = atoi(strtok(NULL, " "));
            break;
        }
    }
    Move move = find_best_move(pos, time);
    printf("bestmove ");
    if (move)
    	move_print(move);
    else
	printf("(none)");
    printf("\n"), fflush(stdout);
}

int main(int argc, char **argv) {
    init();

    Position pos = position();
    if (argc > 1)
        if (strcmp(argv[1], "go") == 0) {
            UCIGo(&pos, argv[1]);
	    return 0;
	}

    char buf[8192] = {0};
    setbuf(stdout, NULL);

    while (GetLine(buf)) {
        if (strcmp(buf, "uci") == 0) {
            printf("id name %s\n", PROJECT_NAME);
            printf("id author %s\n", PROJECT_AUTHOR);
            printf("uciok\n"), fflush(stdout);
        } else if (strcmp(buf, "isready") == 0)
            printf("readyok\n"), fflush(stdout);
        else if (strcmp(buf, "ucinewgame") == 0) {
            tt_clear();
            position_free(&pos);
            pos = position();
        } else if (strstr(buf, "position") == buf) {
            position_free(&pos);
            pos = UCIPosition(buf);
        } else if (strstr(buf, "go") == buf)
            UCIGo(&pos, buf);
        else if (strcmp(buf, "quit") == 0)
            break;
    }
}
