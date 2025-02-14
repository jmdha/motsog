PROJECT_NAME = motsog
PROJECT_AUTHOR = Jan M. D. Hansen
PROJECT_VERSION = 21

CC = gcc
WFLAGS = -Wall -Wextra -Wshadow
CFLAGS = -Isrc -Ofast -std=c17 -march=native -flto -ggdb
DEFINE = -DPROJECT_NAME='"$(PROJECT_NAME)"' -DPROJECT_AUTHOR='"$(PROJECT_AUTHOR)"' -DPROJECT_VERSION='"$(PROJECT_VERSION)"'

SRCS = $(shell find src -type f -iname '*.c')

.PHONY: bench perft test

all: .motsog

for .motsog: CFLAGS += -DNDEBUG
.motsog:
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o motsog bin/uci.c $(SRCS)

for bench: CFLAGS += -DNDEBUG
bench:
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o bench bin/bench.c $(SRCS)

for perft: CFLAGS += -DNDEBUG
perft:
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o perft bin/perft.c $(SRCS)

for test: CFLAGS += -fsanitize=address
test:
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o test bin/test.c $(SRCS)

clean:
	rm -f motsog
	rm -f test
	rm -f bench
	rm -f perft
