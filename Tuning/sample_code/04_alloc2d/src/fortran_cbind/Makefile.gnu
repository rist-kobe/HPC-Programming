## GNU (8.5.0)
FC=gfortran
CC=gcc
FFLAGS=-g -Wall -O0 -cpp -D__USE_GNU__
CFLAGS=-g -Wall -O0
INCPATH= 
LIBPATH= 
LIB= 
###########################################################################
SRC= \
     main.f90

CSRC= \
	wrapper.c

OBJS = \
       $(SRC:.f90=.o)

COBJS = \
	$(CSRC:.c=.o)

.SUFFIXES: .f90 .c .o

run.x: main.o wrapper.o
	$(FC) $(FFLAGS) $(INCPATH) $(LIBPATH) $(OBJS) $(COBJS) $(LIB) -o $@

.f90.o:  
	$(FC) $(FFLAGS) -c $(INCPATH) $< -o $@
.c.o:
	$(CC) $(CFLAGS) -c $(INCPATH) $< -o $@

main.o: main.f90

wrapper.o: wrapper.c


.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f *.x

