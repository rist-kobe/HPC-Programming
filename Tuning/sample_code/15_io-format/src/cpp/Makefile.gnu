## GNU 8.5.0, 9.3.1
CXX=g++
CC=gcc
COPT=-O1
LIB=
INC=
CXXFLAGS=-g -std=gnu++11 $(COPT) $(INC) 
CFLAGS_TIMER=-g -std=gnu11 -O0 $(INC) 

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

.PHONY:
clean:
	rm -f *~ *.o 

.PHONY:
veryclean: clean
	rm -f *.x
