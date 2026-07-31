COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
CXX=g++
CC=gcc
COPT=-O1
LIB=
INC=
CXXFLAGS=-g -std=gnu++11 $(COPT) $(INC) 
CFLAGS_TIMER=-g -std=gnu11 -O0 $(INC)
else ifeq ($(COMPILER),intel)
##Intel DPC++/C++ (Intel oneAPI 2022.3.1)
CXX=icpx
CC=icx
COPT=-O1
LIB=
INC=
CXXFLAGS=-g -std=gnu++11 $(COPT) $(INC) 
CFLAGS_TIMER=-g -std=gnu11 -O0 $(INC)
else ifeq ($(COMPILER),clang)
##LLVM 13.0.0, Apple clang 15.0.0
CXX=clang++
CC=clang
COPT=-O1
LIB=
INC=
CXXFLAGS=-g -std=gnu++11 $(COPT) $(INC) 
CFLAGS_TIMER=-g -std=gnu11 -O0 $(INC)
endif

SRC = \
	main.cpp

OBJ  = $(SRC:.cpp=.o)

EXE=run.x

.SUFFIXES: .o .cpp .c

$(EXE): timer.o $(OBJ)
	$(CXX) $(CXXFLAGS) timer.o $(OBJ) $(LIB) -o $@	

main.o: timer.o main.cpp

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

timer.o:
	$(CC) $(CFLAGS_TIMER) -c timer.c -o $@

.PHONY: clean
clean:
	rm -f *~ *.o 

.PHONY: veryclean
veryclean: clean
	rm -f *.x
