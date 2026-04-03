CC=mpicc
#CFLAGS=-g -c11 -O0 -acc=gpu -gpu=cc80,sm_80 #A100
CFLAGS=-v -g -c11 -O0 -gpu=cc90,sm_90,cuda12.3 -tp=zen3  #H100
#CUDA_ROOT=/opt/nvidia/hpc_sdk/23.5/Linux_x86_64/23.5/cuda
INC=-cuda
LIB= #-L$(CUDA_ROOT)/lib64 -lcudart
LINKOPT=$(CFLAGS) -cuda 

CFLAGS += $(INC)

.SUFFIXES: .o .c

SRC = main.c

OBJ = $(SRC:.c=.o)

EXE=run.x

$(EXE): $(OBJ)
	$(CC) $(LINKOPT) -o $@ $(OBJ) $(LIB)

main.o: main.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:
clean:
	rm -f *.o 

.PHONY:
veryclean: clean
	rm -f $(EXE)
