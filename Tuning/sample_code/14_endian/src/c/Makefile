CC = gcc
CFLAGS = -g -O0 -std=c99
## if you can use 64-bit int
##CFLAGS = -g -O0 -std=c99 #-D__BIGINT__

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

