COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
CC = gcc
CFLAGS = -g -O2 -fopenmp -std=c99
###################################################################
endif

TARGET = run.x

SRC = main.c
OBJS = $(SRC:.c=.o)

.SUFFIXES: .c .o

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

main.o: main.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)
