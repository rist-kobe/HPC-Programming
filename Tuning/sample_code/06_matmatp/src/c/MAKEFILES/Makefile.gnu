## GNU 8.5.0, 9.3.1
CC=gcc
ARCHFLAGS=-mavx2 # x86 with AVX2
COPT=-O2 -fopt-info-all=optinfo.all 
LIB= 
INC=
CFLAGS=-g -Wall -std=gnu11 $(ARCHFLAGS) $(COPT) $(INC) 

CFLAGS_O0=-g -O0 -std=gnu11 $(INC)

SRC = \
	main.c     \
	mykernel.c

OBJ  = $(SRC:.c=.o)

.SUFFIXES: .o .c

all: run.x

run.x: timer.o $(OBJ)
	$(CC) $(CFLAGS) timer.o $(OBJ) $(LIB) -o $@	

main.o: mykernel.o main.c

mykernel.o: mykernel.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

timer.o:
	$(CC) $(CFLAGS_O0) -c timer.c -o $@

.PHONY:
clean:
	rm -f *~ *.o *optinfo*

.PHONY:
veryclean: clean
	rm -f *.x
