COMPILER ?= gnu

ifeq ($(COMPILER),gnu)
## GNU 8.5.0, 9.3.1
CXX=g++

CXXFLAGS=-g -Wall -std=gnu++11  -O0
#LIB=
#######################################################################
else ifeq ($(COMPILER),clang)
## LLVM 13.0.0, Apple clang 15.0.0
CXX=clang++

CXXFLAGS=-g -Wall -std=gnu++11  -O0
#LIB=
#######################################################################
endif

SRC = \
	main.cpp

OBJS = $(SRC:.cpp=.o)

.SUFFIXES: .o .cpp

run.x: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIB) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: main.cpp

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f run.x
