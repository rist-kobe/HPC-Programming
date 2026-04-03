CC=mpicc
#CFLAGS=-g -c11 -O0 -tp=host
#CFLAGS=-g -c11 -O0 -tp=neoverse-v2
CFLAGS=-g -c11 -O0 -tp=zen3
INC=
LIB=

CFLAGS += $(INC)

.SUFFIXES: .o .c

SRC = main.c

OBJ = $(SRC:.c=.o)

EXE=run.x

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIB)

main.o: main.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:
clean:
	rm -f *.o 

.PHONY:
veryclean: clean
	rm -f $(EXE)
