COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
CC = gcc
CFLAGS = -g -std=c99 -O2 -fopenmp #-DUSE_STATICMEM
###################################################################
endif

TARGET = run.x

OBJS = main.o memory.o

.SUFFIXES: .c .o

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

main.o: memory.o main.c

memory.o: memory.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)
