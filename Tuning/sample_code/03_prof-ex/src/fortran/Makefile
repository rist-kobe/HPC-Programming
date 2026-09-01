#
# Preprocessor option
# _DIFFUSE: Boundary condition (not periodic boundary condition)
#

#
# GNU (8.5.0)
#
FC=gfortran

## Select build mode: make [MODE=std|gprof]  (default: gprof)
MODE ?= gprof

BASE_FFLAGS=-g -Wall -cpp -O2 -D_DIFFUSE

ifeq ($(MODE),std)
  ## standard
  FFLAGS=$(BASE_FFLAGS) -fopt-info
else ifeq ($(MODE),gprof)
  ## for using gprof
  FFLAGS=-pg $(BASE_FFLAGS)
else
  $(error Unknown MODE '$(MODE)'. Use MODE=std or MODE=gprof)
endif


COMSRC = \
	common_par.f90       \
	common_var.f90       \
	read_input.f90       \
	write_output_pos.f90 \
	message.f90

RNDSRC = \
	mtfort90.f90 \
	rndvar.f90

CALSRC = \
	calc_param.f90     \
	calc_mass.f90      \
	calc_iniposit.f90  \
	calc_inivel.f90    \
	calc_force.f90     \
	calc_positr.f90

MAIN   = \
	main.f90

OBJS   = \
	$(COMSRC:.f90=.o) \
	$(RNDSRC:.f90=.o) \
	$(CALSRC:.f90=.o) \
	$(MAIN:.f90=.o)

.SUFFIXES: .f90 .o

.PHONY: common rnd calc main

diffuse.x: common rnd calc main
	$(FC) $(FFLAGS) $(OBJS) -o $@ 

# mtfort90.f90 is NOT included in this sample.
# See README.md for instructions on how to obtain it.
mtfort90.f90:
	$(error $@ is missing. Download the Mersenne-Twister source (mtfort90.f90) and copy it into this directory. See README.md for details)

common:
	$(FC) $(FFLAGS) -c $(COMSRC) 

rnd: common mtfort90.f90
	$(FC) $(FFLAGS) -c $(RNDSRC) 

calc: common
	$(FC) $(FFLAGS) -c $(CALSRC)

main: common 
	$(FC) $(FFLAGS) -c main.f90

.PHONY: clean
clean:
	rm -f *.o *.mod *.f90~ *__genmod.f90 gmon.out

.PHONY: veryclean
veryclean: clean
	rm -f *.x
