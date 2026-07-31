COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
CC=gcc
CFLAGS=-g -Wall -std=gnu99 -O1
# To increase optimization level 
#  I think compiler-based optimization would be much better than
#  an application of manually stripmining. :(
#ARCHFLAGS=-mavx2 
#CFLAGS=-g -Wall -std=gnu99 $(ARCHFLAGS) -O2 -ftree-loop-vectorize -fopt-info-all
#######################################################################
else ifeq ($(COMPILER),intel)
## Intel DPC++/C++ (Intel oneAPI 2022.3.1) 
CC=icx
CFLAGS=-g -Wall -std=gnu11 -O1 
# To increase optimization level 
#  I think compiler-based optimization would be much better than
#  an application of manually stripmining. :(
#ARCHFLAGS=-mavx2 # x86, avx2
#ARCHFLAGS=-xCORE-AVX512 # x86, CORE-AVX512
#CFLAGS=-g -Wall -std=gnu11 $(ARCHFLAGS) -O2 -fvectorize -qopt-report #-fsave-optimization-record
#######################################################################
else ifeq ($(COMPILER),intel-classic)
## Intel C/C++ Classic (oneAPI 2022.3.1)
CC=icc
CFLAGS=-g -Wall -std=gnu11 -O1
# To increase optimization level 
#  I think compiler-based optimization would be much better than
#  an application of manually stripmining. :(
#ARCHFLAGS=-mavx2 # xeon, amd64 (with AVX2)
#ARCHFLAGS=-xCORE-AVX512 # xeon (with CORE-AVX512)
#CFLAGS=-g -Wall -std=gnu11 $(ARCHFLAGS) -O2 -qopt-report=5
#######################################################################
endif

SRC = \
	main.c \
	timer.c

OBJS = $(SRC:.c=.o)

.SUFFIXES: .c .o

run.x: $(OBJS)  
	$(CC) $(CFLAGS) $(OBJS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c

timer.o: timer.h timer.c

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f run.x
