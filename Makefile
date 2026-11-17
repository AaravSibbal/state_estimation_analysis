
CC = gcc -std=c11
CFLAGS = -Wall -Wextra -Werror -Wshadow -Wdouble-promotion -g3 -Wfloat-equal -Wcast-align=strict -Wswitch-default -Wswitch-enum -Wconversion -Wpedantic -Wstrict-prototypes -Wmissing-prototypes 


zoro.o: zoro.c zoro.h
	$(CC) $(CFLAGS) -c zoro.c -o zoro.o

main: main.c zoro.o
	$(CC) $(CFLAGS) main.c zoro.o -o main

clean:
	rm -f *.o main.exe
