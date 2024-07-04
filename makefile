PROJECT_NAME = jarlo
PROJECT_AUTHOR = Jan M. D. Hansen
PROJECT_VERSION = 0

CC = clang
WFLAGS = -Wall -Wextra -Wshadow
CFLAGS = -Isrc -g -O3 -std=c17 -march=native -flto
DEFINE = -DPROJECT_NAME='"$(PROJECT_NAME)"' -DPROJECT_AUTHOR='"$(PROJECT_AUTHOR)"' -DPROJECT_VERSION='"$(PROJECT_VERSION)"'

SRCS = $(shell find src -type f -iname '*.c')
OBJS = $(SRCS:.c=.o)

.objs: $(OBJS)

.build: $(OBJS)

for bench: CFLAGS += -DNDEBUG
bench: .objs
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o bench bin/bench.c $(OBJS)

for perft: CFLAGS += -DNDEBUG
perft: .objs
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o perft bin/perft.c $(OBJS)

for test: CFLAGS += -fsanitize=address
test: .objs
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o test bin/test.c $(OBJS)

clean:
	rm -f test
	rm -f bench
	find . -name "*.o" -type f -delete
