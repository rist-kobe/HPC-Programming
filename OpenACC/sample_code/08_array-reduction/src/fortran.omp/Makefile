CC=nvfortran
CFLAGS=-g -O0 -mp=multicore -Minfo=mp

.SUFFIXES: .F90 .o

run.x: main.o 
	$(CC) $(CFLAGS) main.o -o run.x

main.o: main.F90

.F90.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *.mod *.x
