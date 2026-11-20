
CC = gcc -std=c11
CFLAGS = 


zoro.o: zoro.c zoro.h
	$(CC) $(CFLAGS) -c zoro.c -o zoro.o

vec3.o: vec3.c vec3.h
	$(CC) $(CFLAGS) -c vec3.c -o vec3.o

mahony.o: filters\madgwick\MadgwickAHRS.c filters\madgwick\MadgwickAHRS.h
	$(CC) $(CFLAGS) -c filters\madgwick\MadgwickAHRS.c -o mahony.o

mahony.o: filters\my_mahony\MahonyAHRS.c filters\my_mahony\MahonyAHRS.h
	$(CC) $(CFLAGS) -c filters\my_mahony\MahonyAHRS.c -o mahony.o

main: main.c zoro.o
	$(CC) $(CFLAGS) main.c zoro.o -o main

clean:
	rm -f *.o main.exe

CC      := gcc
CFLAGS  := -std=c99 -Wall -Wextra -Werror -Wshadow -Wdouble-promotion -g3 -Wfloat-equal -Wcast-align=strict -Wswitch-default -Wswitch-enum -Wconversion -Wpedantic -Wstrict-prototypes -Wmissing-prototypes 
INC     := -Isrc -Ithird_party/unity
LDLIBS  := -lm

BUILD   := build
SRCS    := $(wildcard src/*.c) third_party/unity/unity.c
TESTS   := $(wildcard test/test_*.c)

# src/quat.c -> build/src/quat.o  (mirrors the source tree)
SRC_OBJS  := $(SRCS:%.c=$(BUILD)/%.o)
TEST_OBJS := $(TESTS:%.c=$(BUILD)/%.o)
TEST_BINS := $(TESTS:test/%.c=$(BUILD)/bin/%)

.PHONY: all test clean
.SECONDARY:   # keep .o files (make deletes chained intermediates otherwise)
all: $(TEST_BINS)

# one executable per test file: its own .o + all source .o files
$(BUILD)/bin/%: $(BUILD)/test/%.o $(SRC_OBJS)
	@mkdir -p $(@D)
	$(CC) $^ $(LDLIBS) -o $@

# generic compile rule; creates the matching subfolder on demand
$(BUILD)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

test: $(TEST_BINS)
	@for t in $^; do echo "== $$t"; ./$$t || exit 1; done

clean:
	rm -rf $(BUILD)

-include $(SRC_OBJS:.o=.d) $(TEST_OBJS:.o=.d)