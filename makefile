PROJECT_NAME = jarlo
PROJECT_AUTHOR = Jan M. D. Hansen
PROJECT_VERSION = 3

CC = gcc
WFLAGS = -Wall -Wextra -Wshadow -pedantic
CFLAGS = -Isrc -g -O3 -std=c17 -march=native -flto
DEFINE = -DPROJECT_NAME='"$(PROJECT_NAME)"' -DPROJECT_AUTHOR='"$(PROJECT_AUTHOR)"' -DPROJECT_VERSION='"$(PROJECT_VERSION)"'

SRCS = $(shell find src -type f -iname '*.c')
OBJS = $(SRCS:.c=.o)

all: .jarlo

.objs: $(OBJS)

.build: $(OBJS)

for .jarlo: CFLAGS += -DNDEBUG
.jarlo: .objs
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o jarlo bin/uci.c $(OBJS) -lm

for bench: CFLAGS += -DNDEBUG
bench: .objs
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o bench bin/bench.c $(OBJS) -lm

for perft: CFLAGS += -DNDEBUG
perft: .objs
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o perft bin/perft.c $(OBJS) -lm

for test: CFLAGS += -fsanitize=address
test: .objs
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o test bin/test.c $(OBJS) -lm

clean:
	rm -f jarlo
	rm -f test
	rm -f bench
	rm -f perft
	find . -name "*.o" -type f -delete
