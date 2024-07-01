PROJECT_NAME = jarlo
PROJECT_AUTHOR = Jan M. D. Hansen
PROJECT_VERSION = 0.0.3

CC = clang
LIBS = -lm
WFLAGS = -std=c17 -Wall -Wextra -Wshadow
CFLAGS = -Isrc -O3 -std=c17 -march=native -flto
DEFINE = -DPROJECT_NAME='"$(PROJECT_NAME)"' -DPROJECT_AUTHOR='"$(PROJECT_AUTHOR)"' -DPROJECT_VERSION='"$(PROJECT_VERSION)"'

SRCS = $(shell find src -type f -iname '*.c')
OBJS = $(SRCS:.c=.o)

.build: $(OBJS)

all: build
build: jarlo

jarlo: .build
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o jarlo bin/uci.c $(OBJS) $(LIBS)

for test: CFLAGS += -fsanitize=address -O0
test: .build
	$(CC) $(CFLAGS) $(WFLAGS) $(DEFINE) -o test_runner tests/*.c $(OBJS) -lcriterion $(LIBS)
	./test_runner

clean:
	rm -f jarlo
	rm -f test_runner
	find . -name "*.o" -type f -delete
