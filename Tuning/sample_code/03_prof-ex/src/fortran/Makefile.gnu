#
# Preprocessor option
# _DIFFUSE: Boundary condition (not periodic boundary condition)
#

#
# GNU (8.5.0)
#
FC=gfortran
## standard 
FFLAGS=-g -Wall -cpp -O2 -fopt-info -D_DIFFUSE
## for using gprof
#FFLAGS=-g -Wall -cpp -pg -O2 -D_DIFFUSE

#####################################################
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

diffuse.x: common rnd calc main
	$(FC) $(FFLAGS) $(OBJS) -o $@ 

common:
	$(FC) $(FFLAGS) -c $(COMSRC) 

rnd: common
	$(FC) $(FFLAGS) -c $(RNDSRC) 

calc: common
	$(FC) $(FFLAGS) -c $(CALSRC)

main: common 
	$(FC) $(FFLAGS) -c main.f90

.PHONY: clean
clean:
	rm -f *.o *.mod *.f90~ *__genmod.f90

.PHONY: clean
veryclean: clean
	rm -f *.x 
