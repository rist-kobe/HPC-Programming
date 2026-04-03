## GNU 8.5.0, 9.3.1
CXX=g++

CXXFLAGS=-g -Wall -std=gnu++11  -O0
#LIB=
#######################################################################
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
