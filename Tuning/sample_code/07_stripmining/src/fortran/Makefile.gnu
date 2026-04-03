## GNU 8.5.0, 9.3.1
FC=gfortran
FFLAGS=-g -Wall -O1 
# To increase optimization level
#  I think compiler-based optimization would be much better than
#  an application of manually stripmining. :(
#ARCHFLAGS=-mavx2 # x86 
#FFLAGS=-g -Wall $(ARCHFLAGS) -O2 -fopt-info-all
CC=gcc
CFLAGS=-g -Wall -O0 -std=gnu11 
#######################################################################
SRC = \
	main.f90

CSRC= \
	timer.c

OBJS = $(SRC:.f90=.o)
COBJS= $(CSRC:.c=.o)


.SUFFIXES: .f90 .c .o

run.x: $(OBJS) $(COBJS) 
	$(FC) $(FFLAGS) $(OBJS) $(COBJS) -o $@

.f90.o:
	$(FC) $(FFLAGS) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.f90

timer.o: timer.h timer.c

.PHONY: clean
clean:
	rm -f *.o *~ *.mod

.PHONY: veryclean
veryclean: clean
	rm -f run.x
