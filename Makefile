
CC = clang

CFLAGS = -std=c11 -g -fno-common

rvcc: main.o
	$(CC) -o rvcc $(CFLAGS) main.o

test: rvcc
	chmod 777 test.sh
	./test.sh 

clean:
	rm -rf rvcc *.o *.s tmp* a.out

.PHONY: test clean