#
# Makefile for the UM
#
CC = gcc

IFLAGS  = -I/comp/40/build/include -I/usr/sup/cii40/include/cii
CFLAGS  = -g -std=gnu99 -Wall -Wextra -Werror -pedantic $(IFLAGS)
LDFLAGS = -g -L/comp/40/build/lib -L/usr/sup/cii40/lib64
LDLIBS  = -l40locality -lcii40 -lm -lbitpack -lcii40 -lum-dis -lcii

EXECS   = um um_test

all: $(EXECS)

um: um.o um_run.o um_parser.o um_seq.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

um_test:  um_test.o um_writetest.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# To get *any* .o file, compile its .c file with the following rule.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECS)  *.o
