CC = gcc
CFLAGS = -g -Wall -std=gnu99 -O3 -fopenmp
LIB =
INC =

TARGET = run.x

OBJS = mykernel.o main.o

.SUFFIXES: .o .c

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INC) $(OBJS) $(LIB) -o $@

main.o: main.c

mykernel.o: mykernel.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *~ *.o

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)
