## GNU 8.5.0, 9.3.1
CC=g++
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
#ARCHFLAGS=-arch arm64 -mcpu=apple-a13 # MacM1 (Arm64)
CFLAGS=-g -std=gnu++11 $(ARCHFLAGS) -O3 -ffast-math -funroll-loops -ftree-vectorize -ftree-loop-vectorize -fopt-info-all=optinfo.all 
###################################################################################
SRC= \
     main.cc

OBJS = \
       $(SRC:.cc=.o)

.SUFFIXES: .cc .o

run.x: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $@

.cc.o:
	$(CC) $(CFLAGS) -c $< $(LIB) -o $@

main.o: main.cc

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f *.x
