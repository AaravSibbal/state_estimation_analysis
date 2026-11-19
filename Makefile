
CC = gcc -std=c11
CFLAGS = -Wall -Wextra -Werror -Wshadow -Wdouble-promotion -g3 -Wfloat-equal -Wcast-align=strict -Wswitch-default -Wswitch-enum -Wconversion -Wpedantic -Wstrict-prototypes -Wmissing-prototypes 


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
