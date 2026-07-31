CXX = g++
CXXFLAGS = -g -O2 -fopenmp #-DWO_CRITICAL or -DBETTER_IMPL
LIB =

TARGET = run.x

OBJS = myclass.o main.o

.SUFFIXES: .cpp .o

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIB) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

main.o: myclass.o main.cpp

myclass.o: myclass.cpp

.PHONY: clean
clean:
	rm -f *.o *~

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)
