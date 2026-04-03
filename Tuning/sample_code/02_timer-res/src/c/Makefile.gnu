## GNU 8.5.0, 9.3.1
CC=gcc
CFLAGS=-Wall -O0 -std=gnu11  
#LIB=
#######################################################################
SRC = \
	timer.c \
	main.c

OBJS = $(SRC:.c=.o)

.SUFFIXES: .o .c

run.x: timer.o main.o 
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
