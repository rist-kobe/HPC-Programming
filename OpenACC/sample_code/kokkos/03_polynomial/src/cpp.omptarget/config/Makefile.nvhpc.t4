CC=nvc++
CFLAGS=-g --c++17 -O3 -mp=gpu -gpu=sm_90,cuda12.3 -tp=zen3 -Minfo=opt,vect,accel,mp
##########################################################################
SRC = \
	main.cpp \
	mykernel.cpp

OBJ = $(SRC:.cpp=.o)

.SUFFIXES: .cpp .o

run.x: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: mykernel.o main.cpp

mykernel.o: mykernel.cpp

.PHONY: clean
clean:
	rm -f *.o *~ *.lst

.PHONY: veryclean
veryclean: clean
	rm -f *.x
