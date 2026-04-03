! Copyright 2024 Research Organization for Information Science and Technology
!!$---------------------------------------------------------------------
!!$ message.f90
!!$ Author: Yukihiro Ota (yota@rist.or.jp)
!!$ Last update: 1 Nov 2016
!!$
!!$---------------------------------------------------------------------

!!$---Subroutine: message_of_header
subroutine message_of_header

  use common_par,only:unit_std_out
  
  implicit none
  
  write(unit_std_out,'(A)') &
       "======================================================="
  write(unit_std_out,'(A)') &
       " diffuse"
  write(unit_std_out,'(A)') &
       "MOLECULAR DYNAMICS METHOD FOR MOLECULAR DIFFUSION PROBLEM"
  write(unit_std_out,'(A)') 
       
  write(unit_std_out,'(A)') &
       "Author: Yukihiro Ota (yota@rist.or.jp)"
  write(unit_std_out,'(A)') &
       "Last update: 1 Nov 2016"
  write(unit_std_out,'(A)') 
       
  write(unit_std_out,'(A)') &
       "The original code is taken from a textbook,"
  write(unit_std_out,'(A)') &
       "A. Sato, Introduction to Molecular simulation with sample"
  write(unit_std_out,'(A)') &
       "simulation programs (in Japanese) (Kyouritsu, 2004) 3.1.6"
  write(unit_std_out,'(A)') 
       
  write(unit_std_out,'(A)') &
       "======================================================="
  write(unit_std_out,'(A)') 

  return
end subroutine message_of_header
!!$---End of Subroutine: message_of_header


!!$---Subroutine: message_of_usage
subroutine message_of_usage

  use common_par,only:unit_std_out
  
  implicit none
 
  call message_of_header

  write(unit_std_out,'(A)') &
       "-------------------------------------------------------"
  write(unit_std_out,'(A)') &
       "[usage] diffuse.x <arg1> "
  write(unit_std_out,'(A)') &
       "  <arg1>: any integer (e.g. 0)"
  write(unit_std_out,'(A)') &
       "          This argument is never used in the program.  "
  write(unit_std_out,'(A)') &
       "-------------------------------------------------------"
  
  write(unit_std_out,'(A)') &
       "<input parameters>"
  write(unit_std_out,'(A)') &
       "num_partot: Total number of particles (NTOT=NA+NB)"
  write(unit_std_out,'(A)') &
       "num_parA: Number of particle A"
  write(unit_std_out,'(A)') &
       "num_parB: Number of particle B"
  write(unit_std_out,'(A)') &
       "nstep: Total number of time steps"
  write(unit_std_out,'(A)') &
       "nobs: Observation step"
  write(unit_std_out,'(A)') 
       
  write(unit_std_out,'(A)') &
       "iflag_test: Flag for test mode (0/1)"
  write(unit_std_out,'(A)') &
       "  0: test mode (no real calculation is performed)"
  write(unit_std_out,'(A)') &
       "  1: simulation mode"
  write(unit_std_out,'(A)') 
       
  write(unit_std_out,'(A)') &
       "mamu_parA: Mass of particle A (atomic mass unit)"
  write(unit_std_out,'(A)') &
       "mamu_parB: Mass of particle B (atomic mass unit)"
  write(unit_std_out,'(A)') &
       "len_cel: Unit-cell length (square box) (Angstrom)"
  write(unit_std_out,'(A)') &
       "tempt: Temperature (K)"
  write(unit_std_out,'(A)') &
       "epsLJ: epsilon of LJ pot (kJ/mol)"
  write(unit_std_out,'(A)') &
       "sigLJ: sigma of LJ pot (Angstrom)"
  write(unit_std_out,'(A)') &
       "dt: Increment of time step (femto second)"
  write(unit_std_out,'(A)') 
       
  return
end subroutine message_of_usage
!!$---End of Subroutine: message_of_usage


!!$---Subroutine: message_of_rephinp
subroutine message_of_rephinp

  use common_par,only: num_partot,num_parA,num_parB,nstep,nobs,&
       iflag_test,mamu_parA,mamu_parB,len_cell,tempt,epsLJ,    &
       sigLJ,dt,unit_std_out
  
  implicit none
  
  write(unit_std_out,'(A)') &
       "[Input parameters]"
  write(unit_std_out,'(A)') &
       "num_partot: Total number of particles (NTOT=NA+NB)"
  write(unit_std_out,'(1I10)') &
       num_partot
  write(unit_std_out,'(A)') &
       "num_parA: Number of particle A"
  write(unit_std_out,'(1I10)') &
       num_parA
  write(unit_std_out,'(A)') &
       "num_parB: Number of particle B"
  write(unit_std_out,'(1I10)') &
       num_parB
  write(unit_std_out,'(A)') &
       "nstep: Total number of time steps"
  write(unit_std_out,'(1I10)') &
       nstep
  write(unit_std_out,'(A)') &
       "nobs: Observation step"
  write(unit_std_out,'(1I10)') &
       nobs
  write(unit_std_out,'(A)') 
       
  write(unit_std_out,'(A)') &
       "iflag_test: Flag for test mode (0/1)"
  write(unit_std_out,'(1I10)') &
       iflag_test
  write(unit_std_out,'(A)') &
       "mamu_parA: Mass of particle A "
  write(unit_std_out,'(1E17.6,1A18)') &
       mamu_parA,"amu"
  write(unit_std_out,'(A)') &
       "mamu_parB: Mass of particle B "
  write(unit_std_out,'(1E17.6,1A18)') &
       mamu_parB,"amu"
  write(unit_std_out,'(A)') &
       "len_cell: Unit-cell length (square box) "
  write(unit_std_out,'(1E17.6,1A18)') &
       len_cell,"Angstrom"
  write(unit_std_out,'(A)') &
       "tempt: Temperature "
  write(unit_std_out,'(1E17.6,1A18)') &
       tempt,"K"
  write(unit_std_out,'(A)') &
       "epsLJ: epsilon of LJ pot "
  write(unit_std_out,'(1E17.6,1A18)') &
       epsLJ,"kJ/mol"
  write(unit_std_out,'(A)') &
       "sigLJ: sigma of LJ pot "
  write(unit_std_out,'(1E17.6,1A18)') &
       sigLJ,"Angstrom"
  write(unit_std_out,'(A)') &
       "dt: Increment of time step "
  write(unit_std_out,'(1E17.6,1A18)') &
       dt,"fs"
  write(unit_std_out,'(A)') 
       
  return
end subroutine message_of_rephinp
!!$---End of Subroutine: message_of_rephinp


!!$---Subroutine: message_of_calcparam
subroutine message_of_calcparam

  use common_par,only: unit_std_out, &
       lunit,vunit,tunit,temptunit,ndensunit,&
       leng,ndens,dtu,nstep_upbd,dt_upbd
 
  implicit none

  write(unit_std_out,'(1A)') &
       "[Calculated parameters]"
  write(unit_std_out,'(1A)') &
       "<Unit of length>"
  write(unit_std_out,'(1E17.6,1A18)') &
       lunit, "(Angstrom)"
  write(unit_std_out,'(1A)') &
       "<Unit of velocity>"
  write(unit_std_out,'(1E17.6,1A18)') &
       vunit, "(m/sec)"
  write(unit_std_out,'(1A)') &
       "<Unit of time>"
  write(unit_std_out,'(1E17.6,1A18)') &
       tunit, "(ns)"
  write(unit_std_out,'(1A)') &
       "<Unit of temperature>"
  write(unit_std_out,'(1E17.6,1A18)') &
       temptunit, "(K)"
  write(unit_std_out,'(1A)') &
       "<Unit of number density>"
  write(unit_std_out,'(1E17.6,1A18)') &
       ndensunit, "(Angstrom)^-3"
  write(unit_std_out,'(A)') 
       
  write(unit_std_out,'(1A)') &
       "<Dimensionless unit-cell length>"
  write(unit_std_out,'(1E17.6)') &
       leng
  write(unit_std_out,'(1A)') &
       "<Dimensionless number density>"
  write(unit_std_out,'(1E17.6)') &
       ndens
  write(unit_std_out,'(1A)') &
       "<Dimensionless time increment>"
  write(unit_std_out,'(1E17.6)') &
       dtu
  write(unit_std_out,'(1A)') 
       
  write(unit_std_out,'(1A)') &
       "The following two quantities are useful for"
  write(unit_std_out,'(1A)') &
       "tuning <nstep> and <dt>."
  write(unit_std_out,'(1A)') &
       "<Characteristic number of time steps>"
  write(unit_std_out,'(1A)') &
       "(unit-cell-length/2)/(dt*sqrt(3*temperature))="
  write(unit_std_out,'(1E17.6)') &
       nstep_upbd
  write(unit_std_out,'(1A)') &
       "<Characteristic dimensionless time increment>"
  write(unit_std_out,'(1A)') &
       "0.1/sqrt(3*temperature)="
  write(unit_std_out,'(1E17.6)') &
       dt_upbd
  write(unit_std_out,'(A)') 

  return
end subroutine message_of_calcparam
!!$---End of Subroutine: message_of_calcparam


!!$---Subroutine: message_of_output
subroutine message_of_output

  use common_par,only: unit_std_out
  
  implicit none

  write(unit_std_out,'(A)') &
       "[Explanation on output files]"
  write(unit_std_out,'(A)') &
       "+ inigeom.xyz"
  write(unit_std_out,'(A)') &
       "Initial configuration of particle positions is shown"
  write(unit_std_out,'(A)') &
       "with the XYZ format. Each coordinate is dimensionless."
  write(unit_std_out,'(A)') 
       
  write(unit_std_out,'(A)') &
       "+ trj.xyz"
  write(unit_std_out,'(A)') &
       "Trajectory of particle positions is shown"
  write(unit_std_out,'(A)') &
       "with the XYZ format. Each coordinate is dimensionless"
  write(unit_std_out,'(A)') &
       "The stepsize of the trajectory is controlled by the "
  write(unit_std_out,'(A)') &
       "input parameter, <nobs>."
  write(unit_std_out,'(A)') 
       

  return
end subroutine message_of_output
!!$---End of Subroutine: message_of_output


!!$---Subroutine: message_of_testmode
subroutine message_of_testmode

  use common_par,only: unit_std_out
    
  implicit none
  
  call message_of_header

  call message_of_rephinp

  write(unit_std_out,'(A)') &
       "--TEST-MODE-TEST-MODE-TEST-MODE-TEST-MODE-TEST-MODE--"

  call message_of_calcparam

  call message_of_output
  
  return
end subroutine message_of_testmode
!!$---End of Subroutine: message_of_testmode


!!$=--Subroutine: message_of_abtermination
subroutine message_of_abtermination

  use common_par,only: unit_std_out
  
  implicit none

  write(unit_std_out,'(A)') &
       "*** The simulation is abnormally terminated ***"
  return
end subroutine message_of_abtermination
!!$=--End of Subroutine: message_of_abtermination

