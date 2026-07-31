CC=g++
ARCHFLAGS=-mavx2 # x86-64 CPUs with AVX2 support
CFLAGS=-g -std=gnu++11 $(ARCHFLAGS) -O3 -ffast-math -funroll-loops -ftree-vectorize -ftree-loop-vectorize -fopt-info-all=optinfo.all 

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
	rm -f *.o *~ optinfo.all

.PHONY: veryclean
veryclean: clean
	rm -f *.x
