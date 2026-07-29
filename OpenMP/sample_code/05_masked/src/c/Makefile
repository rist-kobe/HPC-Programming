COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
CC = gcc
DFLAGS := $(DFLAGS)
#DFLAGS := -DNOT_USE_MASKED # for GNU 8.5.0
CFLAGS = -g -fopenmp -O0 $(DFLAGS)
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

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.c

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)
