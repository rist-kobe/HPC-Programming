## Nvidia HPC SDK
FC = nvfortran
## Offload mode: omp (OpenMP target offload), acc (OpenACC), nogpu (host only)
OFFLOAD ?= omp
## Target CPU type (e.g., TP=zen2 for AMD EPYC Zen2)
TP ?= native
## GPU code generation flags (override, e.g. GPUFLAGS='-gpu=cc86,cuda11.6,ptxinfo,pinned')
GPUFLAGS ?= -gpu=cc80,ptxinfo,pinned
FFLAGS = -g -Wall -O3
LIB =
INC =
FFLAGS += $(INC) -tp=$(TP)
ifeq ($(OFFLOAD),omp)
  ## OpenMP GPU offload build
  FFLAGS += -mp=gpu -Minfo=mp,accel
  FFLAGS += $(GPUFLAGS)
  ## Example (NVIDIA RTX A5000 / CC 8.6)
  #GPUFLAGS = -gpu=cc86,cuda11.6,ptxinfo,pinned
else ifeq ($(OFFLOAD),acc)
  ## OpenACC GPU offload build
  FFLAGS += -acc=gpu -Minfo=accel
  FFLAGS += $(GPUFLAGS)
  ## Example (NVIDIA RTX A5000 / CC 8.6)
  #GPUFLAGS = -gpu=cc86,cuda11.6,ptxinfo,pinned
else ifeq ($(OFFLOAD),nogpu)
  ## Host-only build (no GPU offloading; keep OpenMP enabled)
  FFLAGS += -mp -Minfo=mp
else
  $(error Unknown OFFLOAD=$(OFFLOAD) (expected omp, acc, or nogpu))
endif

TARGET = run.x

OBJS = mytype.o mykernel.o timer.o main.o

.SUFFIXES: .o .F90

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(FC) $(FFLAGS) $(OBJS) $(LIB) -o $@

main.o: timer.o mytype.o mykernel.o main.F90

mykernel.o: mytype.o mykernel.F90

mytype.o: mytype.F90

timer.o: mytype.o timer.F90

.F90.o:
	$(FC) $(FFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *~ *.o *.mod

.PHONY: veryclean
veryclean: clean
	rm -f $(TARGET)
