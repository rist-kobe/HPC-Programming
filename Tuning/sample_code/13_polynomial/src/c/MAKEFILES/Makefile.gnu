## GNU 8.5.0, 9.3.1
CC=gcc
ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
#ARCHFLAGS=-arch arm64 -mcpu=apple-a13 # MacM1 (Arm64)
CFLAGS=-g -std=gnu99 $(ARCHFLAGS) -O3 -ffast-math -funroll-loops -ftree-vectorize -ftree-loop-vectorize -fopt-info-all=optinfo.all 
##########################################################################
SRC= \
     main.c mykernel.c

OBJS = \
       $(SRC:.c=.o)

.SUFFIXES: .c .o

run.x: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: mykernel.o main.c

main.o: mykernel.c

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f *.x

