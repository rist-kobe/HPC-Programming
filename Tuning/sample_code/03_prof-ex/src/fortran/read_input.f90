! Copyright 2024 Research Organization for Information Science and Technology
!!$---------------------------------------------------------------------
!!$ read_input.f90
!!$ Author: Yukihiro Ota (yota@rist.or.jp)
!!$ Last update: 1 Nov 2016
!!$
!!$---------------------------------------------------------------------

!!$--- Subroutine: read_input
subroutine read_input (icon,iunit)
  
  use common_par,only: num_partot,num_parA,num_parB,nstep,nobs,&
       iflag_test,mamu_parA,mamu_parB,len_cell,tempt,epsLJ,    &
       sigLJ,dt,unit_std_out

  implicit none
  integer,intent(inout) :: icon
  integer,intent(in) :: iunit

  !! /* local variables */
  integer :: nn
  character(len=64) :: cbuff
  
  read(iunit,*) cbuff, num_partot
  read(iunit,*) cbuff, num_parA
  read(iunit,*) cbuff, num_parB
  read(iunit,*) cbuff, nstep
  read(iunit,*) cbuff, nobs
  read(iunit,*) cbuff, iflag_test
  read(iunit,*) cbuff, mamu_parA
  read(iunit,*) cbuff, mamu_parB
  read(iunit,*) cbuff, len_cell
  read(iunit,*) cbuff, tempt
  read(iunit,*) cbuff, epsLJ
  read(iunit,*) cbuff, sigLJ
  read(iunit,*) cbuff, dt

  nn = num_parA + num_parB - num_partot
  if ( nn .ne. 0 ) then
     write(unit_std_out,'(A)')&
          "NTOT must be equal to NA + NB."
     icon = -1
     return
  end if
  
  icon = 0
  return
end subroutine read_input
!!$--- End of Subroutine: read_input


!!$--- Subroutine: chk_input
subroutine chk_input(icon,inpfn)

  use common_par,only: unit_std_out
  
  implicit none
  integer,intent(inout) :: icon
  character(len=32),intent(out) :: inpfn

  !! /* local variables */
  logical :: l_open, l_exist

  !! /* Check number of arguments */
  !icon=iargc() !! this is obsolue (fortran 77)
  icon = command_argument_count()
  if ( icon .ne. 1 ) then
     call message_of_usage()
     icon = 1
     return
  end if
  
  !! /* Check the presence of an input file */
  inpfn="input.dat"
  inquire (FILE=inpfn,OPENED=l_open,EXIST=l_exist,&
             NUMBER=icon)
  if ( .not. l_exist ) then
     write(unit_std_out,'(A)') "Significant errors occur."
     write(unit_std_out,'(2A)') "No input file, named by ", inpfn
     icon = -1
     return
  end if
  
  icon = 0
  return
end subroutine chk_input
!!$--- End of Subroutine: chk_input
