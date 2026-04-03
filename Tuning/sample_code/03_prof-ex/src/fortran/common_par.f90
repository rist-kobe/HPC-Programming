! Copyright 2024 Research Organization for Information Science and Technology
!!$---------------------------------------------------------------------
!!$ common_par.f90
!!$ Author: Yukihiro Ota (yota@rist.or.jp)
!!$ Last update: 1 Nov 2016
!!$
!!$ set common parameters and constants
!!$---------------------------------------------------------------------

module common_par

  implicit none

  !!-------------------------------------------------------------------
  !! /* input parameter */
  !!-------------------------------------------------------------------
  ! Total number of particles (NTOT=NA+NB)
  integer :: num_partot
  ! Number of 'A' particles
  integer :: num_parA
  ! Number of 'B' particles
  integer :: num_parB
  ! Total number of time steps
  integer :: nstep
  ! Observation step
  integer :: nobs
  ! Flag for a test-run mode
  integer :: iflag_test

  ! Mass of particle A (atomic mass unit)
  double precision :: mamu_parA
  ! Mass of particle B (atomic mass unit)
  double precision :: mamu_parB
  ! Unit-cell length (Angstrom)
  double precision :: len_cell
  ! Temperature (K)
  double precision :: tempt
  ! epsilon of Lennard-Jones potential (kJ/mol)
  double precision :: epsLJ
  ! sigma of Lennard-Jones potential (Angstrom)
  double precision :: sigLJ
  ! Increment of time step (femto second)
  double precision :: dt

  
  !!-------------------------------------------------------------------
  !! /* constants */
  !!-------------------------------------------------------------------
  ! device numbers
  integer,parameter :: unit_std_inp = 5
  integer,parameter :: unit_std_out = 6 
  integer,parameter :: unit_gen_inp = 10 
  integer,parameter :: unit_gen_out = 11 
  
  ! numeric constants
  double precision,parameter :: pi  = 3.141592653589793d0
  double precision,parameter :: dpi = 6.283185307179586d0
  double precision,parameter :: rpi = 0.318309886183791d0
  double precision,parameter :: hpi = 1.570796326794897d0
    
  ! Avogadro number (1/mol)
  double precision,parameter :: NA = 6.022140857d23

  ! Boltzmann constant (kJ/(mod K))
  double precision,parameter :: KB = 8.3144621d-3

  ! atomic mass unit (kg)
  double precision,parameter :: MAMU = 1.66053904d-27
  
  ! LJ parameter
  double precision,parameter :: LJ24 = 24.0d0

  ! LJ cutoff distance (dimensionless)
  double precision,parameter :: RC = 3.0d0
  double precision,parameter :: RCSQ = 9.0d0

  !!-------------------------------------------------------------------
  !! /* induced constants */
  !!-------------------------------------------------------------------
  ! unit of length = sigLJ
  double precision :: lunit
  ! unit of velocity = sqrt(epsLJ/MAMU*NA)
  double precision :: vunit
  ! unit of time = sigLJ*sqrt(MAMU*NA/epsLJ)
  double precision :: tunit
  ! unit of temperature = epsLJ/KB
  double precision :: temptunit
  ! unit of number of density = sigLJ^-3
  double precision :: ndensunit
  ! Dimensionless unit-cell length
  double precision :: leng
  ! Dimensionless number density
  double precision :: ndens
  ! Dimensionless time increment
  double precision :: dtu
  ! (dtu)^2
  double precision :: dtusq
  ! Upper bound of number of time steps
  double precision :: nstep_upbd
  ! Upper bound of time increment
  double precision :: dt_upbd
    
end module common_par
