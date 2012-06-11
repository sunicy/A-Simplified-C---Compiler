# Made by Sunicy, Mar. 2012 */

CC=gcc
CFLAGS=-g
COMPILER_NAME=cmmc

default: syntax.tab.o main.o gtree.o stable.o semantic.o \
			interm.o imnode.o im_reduce.o gencode.o
	$(CC) -o $(COMPILER_NAME) syntax.tab.o main.o gtree.o \
			stable.o semantic.o interm.o imnode.o im_reduce.o \
			gencode.o

syntax.tab.o: lexical.c syntax.tab.c
	$(CC) $(CFLAGS) -c -lfl syntax.tab.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

gtree.o: gtree.c
	$(CC) $(CFLAGS) -c gtree.c

stable.o: stable.c
	$(CC) $(CFLAGS) -c stable.c

semantic.o: semantic.c
	$(CC) $(CFLAGS) -c semantic.c

interm.o: interm.c
	$(CC) $(CFLAGS) -c interm.c

imnode.o: imnode.c
	$(CC) $(CFLAGS) -c imnode.c

im_reduce.o: im_reduce.c
	$(CC) $(CFLAGS) -c im_reduce.c

gencode.o: gencode.c
	$(CC) $(CFLAGS) -c gencode.c

lexical.c: lexical.l 
	flex -o lexical.c lexical.l

syntax.tab.c: syntax.y
	bison -dv syntax.y

clean:
	rm -f lexical.c syntax.tab.h syntax.tab.c syntax.output *.o $(COMPILER_NAME)

all: 
	make clean
	make

test:
	./$(COMPILER_NAME) test.txt

g:
	gdb --args $(COMPILER_NAME) mytest.txt
