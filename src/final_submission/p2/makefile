CC = gcc
CFLAGS = -lm -pedantic -Wall -ansi
DEPS = *.h

makemain: main.c *.c
	$(CC) -g -o main *.c $(CFLAGS)

clean:
	rm main
