## GNU 8.5.0, 9.3.1
CC=gcc
CFLAGS=-g -Wall -std=gnu99 -O1
# To increase optimization level 
#  I think compiler-based optimization would be much better than
#  an application of manually stripmining. :(
#ARCHFLAGS=-mavx2 
#CFLAGS=-g -Wall -std=gnu99 $(ARCHFLAGS) -O2 -ftree-loop-vectorize -fopt-info-all
#######################################################################
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
