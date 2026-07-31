CC=gcc
CFLAGS=-g -Wall -O0 -std=gnu99
LIB=

SRC= \
     main.c mykernel.c

OBJS = \
       $(SRC:.c=.o)

.SUFFIXES: .c .o

run.x: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: mykernel.o main.c

mykernel.o: mykernel.c

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f *.x
