! Copyright 2025 Research Organization for Information Science and Technology
! Reference
! * OpenACC Getting started guid, 2.6 and 2.9;
!   https://docs.nvidia.com/hpc-sdk/compilers/openacc-gs/index.html
program main
  use mymod
  implicit none
  integer :: nsize, ierr, nitr 
  integer :: i, itr
  logical :: ldev
  character(len=32) :: cbuf
  real(kind=DP) :: h_x, h_y, h_z, chk

  ierr = command_argument_count ()
  if ( ierr .ne. 1 ) then
     write (6,'(1a32)') "[usage] run.x $1"
     write (6,'(1a32)') "   $1: vector size (integer)"
     stop
  end if

  call get_command_argument (number=1, value=cbuf)
  read (cbuf,'(1I10)') nsize
  if ( nsize .le. 0 ) then
     nsize = 10000 
  end if

  nitr = 100

  s%nsize = nsize
  allocate( s%x(1:nsize) )
  allocate( s%y(1:nsize) )
  allocate( s%z(1:nsize) )

  do i = 1, nsize
    s%x(i) = 1.0_DP / nitr
    s%y(i) = 2.0_DP / nitr
    s%z(i) = 0.0_DP
  end do

  h_x = 1.0_DP / nitr 
  h_y = 2.0_DP / nitr 
  h_z = 0.0_DP

  do itr = 1, nitr 
    h_z = h_x + h_y
  end do

  !---------------------------------------------------------------------
  ! Start 
  !---------------------------------------------------------------------
  !$ACC enter data copyin(s%x(1:nsize),s%y(1:nsize),s%z(1:nsize))

  do itr = 1, nitr
     !$ACC kernels loop present(s,s%x(1:nsize),s%y(1:nsize),s%z(1:nsize))
     do i = 1, nsize
        s%z(i) = s%x(i) + s%y(i)
     end do
  end do

  !$ACC exit data copyout(s%z(1:nsize)) 

  chk = s%z(nsize/2) - h_z
  write (6,'("Diff device and host",1E22.8)') chk
  !---------------------------------------------------------------------
  ! END 
  !---------------------------------------------------------------------

  stop
end program main
