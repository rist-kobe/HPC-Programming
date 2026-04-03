# NVIDIA HPC SDK 23.11 in Xeon(IceLake) and A100
DFLAGS ?=
DFLAGS := $(DFLAGS)

CC=nvc++
CCFLAGS=-g -O2 --c++17 -acc=gpu -gpu=cc80,sm_80,cuda12.3 -Minfo=accel
INC=$(DFLAGS)
LIB=

CCFLAGS += $(INC) 
LINKOPT = $(CCFLAGS) #-cudalib=

EXE = stream

SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

.SUFFIXES: .cpp .o

$(EXE): $(OBJ)
	$(CC) $(LINKOPT) -o $(EXE) $(OBJ) $(LIB)

.cpp.o:
	$(CC) $(CCFLAGS) -c -o $@ $<

main.o: main.cpp

.PHONY:
clean:
	rm -rf *.o $(EXE)

