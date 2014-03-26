CC=gcc
CFLAGS=-Wall -Werror -Iinclude
LDFLAGS=-lpthread
BIN=apricot

.PHONY=clean

apricot: $(patsubst %.c,%.o,$(wildcard *.c))
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	@rm -f *.o 
