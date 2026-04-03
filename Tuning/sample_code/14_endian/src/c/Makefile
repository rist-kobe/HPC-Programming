## GNU 8.5.0
CC=gcc
CCFLAG=-g -O0 -std=c99 
## if you can use 64-bit int
##CCFLAG=-g -O0 -std=c99 #-D__BIGINT__


SRC=\
	main.c

OBJ=$(SRC:.c=.o)

.SUFFIXES: .c .o

run.x: $(OBJ)
	$(CC) $(CCFLAG) $(OBJ) -o $@

.c.o:
	$(CC) $(CCFLAG) -c $< -o $@

main.o: main.c

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f run.x

