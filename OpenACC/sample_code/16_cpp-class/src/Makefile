# NVIDIA HPC SDK 24.1 in AMD EPYC and H100
CC=nvc++
CFLAGS=-g -O2 -acc=gpu -gpu=cc90,sm_90,cuda12.3 -Minfo=accel
INC = 
LIB =

CFLAGS += $(INC)

SRC = main.cpp

OBJ = $(SRC:.cpp=.o)

EXE = run.x

.SUFFIXES: .cpp .o

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIB)

.cpp.o:
	$(CC) $(CFLAGS) -c -o $@ $< 

main.o: main.cpp

.PHONY:
clean:
	rm -f *.o $(EXE)
