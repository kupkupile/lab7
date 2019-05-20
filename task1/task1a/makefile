all: d

d: d.o
		gcc -g -m32 -Wall -fno-pic digit.c -o d

.PHONY: clean

clean: rm -f *.o d