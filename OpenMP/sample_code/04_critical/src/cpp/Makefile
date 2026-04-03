# GNU 8.5.0
CXX=g++
CXXFLAGS=-g -O2 -fopenmp #-DWO_CRITICAL
LIB=

EXE=run.x
OBJS=myclass.o main.o

.SUFFIXES: .cpp .o

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIB) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: myclass.o main.cpp

myclass.o: myclass.cpp

.PHONY:
clean:
	rm -f *.o $(EXE)
