COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0
CC=gcc
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
COPT=-O1 
LIB=-lm
INC=
CFLAGS=-g -std=gnu11 $(ARCHFLAGS) $(COPT) $(INC) 
CFLAGS_TIMER=-g -std=gnu11 -O0 $(INC)
else ifeq ($(COMPILER),intel)
## Intel DPC++/C++ (Intel oneAPI 2022.3.1)
CC=icx
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
COPT=-O1 
LIB=-lm
INC=
CFLAGS=-g -std=gnu11 $(ARCHFLAGS) $(COPT) $(INC) 
CFLAGS_TIMER=-g -std=gnu11 -O0 $(INC)
else ifeq ($(COMPILER),clang)
## LLVM 13.0.0
CC=clang
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
COPT=-O1 
LIB=-lm
INC=
CFLAGS=-g -std=gnu11 $(ARCHFLAGS) $(COPT) $(INC) 
CFLAGS_TIMER=-g -std=gnu11 -O0 $(INC)
endif

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

mekernel.o: mykernel.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

timer.o:
	$(CC) $(CFLAGS_TIMER) -c timer.c -o $@

.PHONY:
clean:
	rm -f *~ *.o 

.PHONY:
veryclean: clean
	rm -f *.x
