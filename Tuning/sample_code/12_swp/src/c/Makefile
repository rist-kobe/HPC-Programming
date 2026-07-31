CC=gcc
ARCHFLAGS=-mavx2 # x86-64 CPUs with AVX2 support
COPT=-O1 
LIB=-lm
INC=
CFLAGS=-g -std=gnu11 $(ARCHFLAGS) $(COPT) $(INC) 
CFLAGS_TIMER=-g -std=gnu11 -O0 $(INC)

SRC = \
	main.c \
	memory.c \
	mykernel.c

OBJ  = $(SRC:.c=.o)

EXE=run.x

.SUFFIXES: .o .c

$(EXE): timer.o $(OBJ)
	$(CC) $(CFLAGS) timer.o $(OBJ) $(LIB) -o $@	

main.o: timer.o mykernel.o main.c

memory.o: memory.c

mykernel.o: mykernel.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

timer.o:
	$(CC) $(CFLAGS_TIMER) -c timer.c -o $@

.PHONY: clean
clean:
	rm -f *~ *.o 

.PHONY: veryclean
veryclean: clean
	rm -f *.x
