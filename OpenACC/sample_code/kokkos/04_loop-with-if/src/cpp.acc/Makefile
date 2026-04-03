CC=nvc++
CFLAGS=-g --c++17 -O3 -acc=gpu -gpu=cc90,sm_90,cuda12.3 -Minfo=accel 
LIB=

.SUFFIXES: .o .cpp

SRC = main.cpp mykernel.cpp timer.cpp

OBJ = $(SRC:.cpp=.o)

EXE=run.x

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIB)

mykernel.o: mykernel.cpp

main.o: mykernel.o timer.o main.cpp

timer.o: timer.cpp

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:
clean:
	rm -f *.o *.lst

.PHONY:
veryclean: clean
	rm -f $(EXE)
