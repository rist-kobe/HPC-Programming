COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
CC=gcc
CFLAGS=-Wall -O0 -std=gnu11  
#LIB=
#######################################################################
endif

SRC = \
	timer.c \
	main.c

OBJS = $(SRC:.c=.o)

.SUFFIXES: .o .c

run.x: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c

timer.o: timer.c timer.h

.PHONY: clean
clean:
	rm -f *.o *~ 

.PHONY: veryclean
veryclean: clean
	rm -f run.x
