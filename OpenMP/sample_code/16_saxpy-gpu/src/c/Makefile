## Nvidia HPC SDK
CC = nvc
## Offload mode: omp (OpenMP target offload), acc (OpenACC), nogpu (host only)
OFFLOAD ?= omp
## Target CPU type (e.g., TP=zen2 for AMD EPYC Zen2)
TP ?= native
## GPU code generation flags (override, e.g. GPUFLAGS='-gpu=cc86,cuda11.6,ptxinfo,pinned')
GPUFLAGS ?= -gpu=cc80,ptxinfo,pinned
CFLAGS = -g -Wall -c99 -O3
LIB =
INC =
CFLAGS += $(INC) -tp=$(TP)
ifeq ($(OFFLOAD),omp)
  ## OpenMP GPU offload build
  CFLAGS += -mp=gpu -Minfo=mp,accel
  CFLAGS += $(GPUFLAGS)
  ## Example (NVIDIA RTX A5000 / CC 8.6)
  #GPUFLAGS = -gpu=cc86,cuda11.6,ptxinfo,pinned
else ifeq ($(OFFLOAD),acc)
  ## OpenACC GPU offload build
  CFLAGS += -acc=gpu -Minfo=accel
  CFLAGS += $(GPUFLAGS)
  ## Example (NVIDIA RTX A5000 / CC 8.6)
  #GPUFLAGS = -gpu=cc86,cuda11.6,ptxinfo,pinned
else ifeq ($(OFFLOAD),nogpu)
  ## Host-only build (no GPU offloading; keep OpenMP enabled)
  CFLAGS += -mp -Minfo=mp
else
  $(error Unknown OFFLOAD=$(OFFLOAD) (expected omp, acc, or nogpu))
endif

TARGET = run.x

OBJS = mykernel.o main.o timer.o

.SUFFIXES: .o .c

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $@

main.o: main.c

mykernel.o: mykernel.c

timer.o: timer.c

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *~ *.o

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)
