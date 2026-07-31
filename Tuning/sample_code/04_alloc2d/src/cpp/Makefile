CXX = g++
CXXFLAGS = -g -Wall -O0 -std=c++11

TARGET = run.x

SRC = \
	main.cc

OBJS = $(SRC:.cc=.o)

.SUFFIXES: .cc .o

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cc

.PHONY: clean
clean:
	rm -f *.o *.cc~ *~

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)
