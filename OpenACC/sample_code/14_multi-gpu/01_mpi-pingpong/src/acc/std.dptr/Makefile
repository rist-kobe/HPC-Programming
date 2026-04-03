CC=mpicc
#CFLAGS=-g -c11 -O0 -acc=gpu -gpu=cc80,sm_80 -tp=host #A100
#CFLAGS=-g -c11 -O0 -acc=gpu -gpu=cc90,sm_90 -tp=neoverse-v2 -Minfo=accel  #GH200
CFLAGS=-g -c11 -O0 -acc=gpu -gpu=cc90,sm_90,cuda12.3 -tp=zen3 -Minfo=accel  #H100
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
	rm $(EXE)
