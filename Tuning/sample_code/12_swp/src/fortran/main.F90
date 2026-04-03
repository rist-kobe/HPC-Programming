! Copyright 2024 Research Organization for Information Science and Technology
! ======================================================================
! Example of software pipelining 
!
! Authors    : Yukihiro Ota (yota@rist.or.jp)
! Last update: 15th Feb. 2024
! ======================================================================
program main
  use mytype,only: DP
  use mykernel,only: coremgs, coremgs_swp, dummy
  use timer,only: get_elp_time
  implicit none

  integer,parameter :: NITR_MAX=40000
  integer,parameter :: NDIM_DEFAULT=120
  integer,parameter :: NDIM_MIN=10
  integer,parameter :: NDIM_MAX=2000

  integer :: i, j, itr, NITR
  integer :: ndim, mvec, icon, ielp0

  character(len=128) :: cbuf

  real(kind=DP) :: vscale
  real(kind=DP) :: elp(2)
  real(kind=DP),allocatable,dimension(:) :: A
  real(kind=DP),allocatable,dimension(:,:) :: Q

  icon = command_argument_count()
  if ( icon .ne. 1 ) then
     write(6,'("[usage] run.x (arg1)",/,&
&             "   (arg1): Dimension of vector (integer)")')
     stop
  end if

  call get_command_argument(1, cbuf)
  read(cbuf,'(1I10)') ndim
  if ( ndim .le. 0 ) then
     ndim = NDIM_DEFAULT
  end if
  if ( ndim .lt. NDIM_MIN ) then
     write(6,'("[ERROR] (arg1) must be larger than ",1I3)') NDIM_MIN
     stop
     !error stop !! for Fortran 2008
  end if
  if ( ndim .gt. NDIM_MAX ) then
     write(6,'("[ERROR] (arg1) must be smaller than ",1I4)') NDIM_MAX
     stop 
     !error stop !! for Fortran 2008
  end if

  ! memory allocation
  allocate( A(1:ndim) ) 
  allocate( Q(1:ndim, 1:ndim) )

  ! initialization
  do j = 1, ndim
  do i = 1, ndim
     Q(i,j) = 0.0_DP
  end do
  end do
  
  vscale = ndim*1.0_DP
  vscale = 1.0_DP / (vscale*vscale)

  !--------------------------------------------------------------------
  BLOCK
  !--------------------------------------------------------------------
  integer :: nseed
  integer,allocatable,dimension(:) :: seed

  real(kind=DP) :: rnd

  call random_seed(size=nseed)
  allocate( seed(1:nseed) )

  do i = 1, nseed
     seed(i) = 409101 + i - 1
  end do

  call random_seed(put=seed)

  do j = 1, ndim
     A(j) = vscale 
    
     do i = 1, j
        call random_number(rnd)
        Q(i,j) = rnd * vscale
     end do

  end do

  deallocate( seed )
  !--------------------------------------------------------------------
  END BLOCK
  !--------------------------------------------------------------------

  if ( ndim .lt. 300 ) then
     NITR = NITR_MAX
  else if ( ndim .lt. 600 ) then
     NITR = NITR_MAX/2
  else if ( ndim .lt. 1200 ) then
     NITR = NITR_MAX/4
  else
     NITR = 1000
  end if

  write(6,'(                                                                           &
&      "> Legends in output",/,                                                        &
&      "> * kernel          : Types of kernels",/,                                     &
&      "> * ndim            : Dimension of vector (innermost-loop length)",/,          &
&      "> * mvec            : Outermost-loop length; target for software pipelining",/,&
&      "> * NITR            : Number of iterations",/,                                 &
&      "> * Elapsed_time_sec: Elapsed time (sec.)",/,                                  &
&      "> * dummy           : Dummy output (no information)")')

  write(6,'(1A16,3A8,1A18,1A8)') "kernel", "ndim", "mvec", "NITR",&
&       "Elapsed_time_sec", "dummy"

  !--------------------------------------------------------------------
  ! large mvec
  !--------------------------------------------------------------------
  mvec = ndim - 1

  ! not software pipelining
  call get_elp_time(ielp0) 

  icon = 0.0_DP
  do itr = 1, NITR
     call coremgs(A, Q, ndim, mvec)
     icon = icon + dummy(A, ndim)

     do j = 1, ndim
        A(j) = vscale 
     end do
  end do

  call get_elp_time(ielp0, elp(1))

  ! software pipelining
  call get_elp_time(ielp0) 

  icon = 0.0_DP
  do itr = 1, NITR
     call coremgs_swp(A,Q,ndim,mvec)
     icon = icon + dummy(A, ndim)

     do j = 1, ndim
        A(j) = vscale 
     end do
  end do
  call get_elp_time(ielp0, elp(2))

  write(6,'(1A16,3I8,1F18.3,1I8)') "coremgs", ndim, mvec, NITR,         &
&       elp(1), icon
  write(6,'(1A16,3I8,1F18.3,1I8)') "coremgs_swp", ndim, mvec, NITR,     &
&       elp(2), icon

  !--------------------------------------------------------------------
  ! medium mvec
  !--------------------------------------------------------------------
  mvec = ndim / 2

  ! not software pipelining
  call get_elp_time(ielp0) 

  icon = 0.0_DP
  do itr = 1, NITR
     call coremgs(A,Q,ndim,mvec)
     icon = icon + dummy(A, ndim)

     do j = 1, ndim
        A(j) = vscale 
     end do
  end do

  call get_elp_time(ielp0, elp(1))

  ! software pipelining
  call get_elp_time(ielp0) 

  icon = 0.0_DP
  do itr = 1, NITR
     call coremgs_swp(A,Q,ndim,mvec)
     icon = icon + dummy(A, ndim)

     do j = 1, ndim
        A(j) = vscale 
     end do
  end do

  call get_elp_time(ielp0, elp(2))

  write(6,'(1A16,3I8,1F18.3,1I8)') "coremgs", ndim, mvec, NITR,         &
&       elp(1), icon
  write(6,'(1A16,3I8,1F18.3,1I8)') "coremgs_swp", ndim, mvec, NITR,     &
&       elp(2), icon

  !--------------------------------------------------------------------
  ! small mvec
  !--------------------------------------------------------------------
  mvec = ndim / 4

  ! not software pipelining
  call get_elp_time(ielp0) 

  icon = 0.0_DP
  do itr = 1, NITR
     call coremgs(A,Q,ndim,mvec)
     icon = icon + dummy(A, ndim)

     do j = 1, ndim
        A(j) = vscale 
     end do
  end do

  call get_elp_time(ielp0, elp(1))

  ! software pipelining
  call get_elp_time(ielp0) 

  icon = 0.0_DP
  do itr = 1, NITR
     call coremgs_swp(A,Q,ndim,mvec)
     icon = icon + dummy(A, ndim)

     do j = 1, ndim
        A(j) = vscale 
     end do
  end do

  call get_elp_time(ielp0, elp(2))

  write(6,'(1A16,3I8,1F18.3,1I8)') "coremgs", ndim, mvec, NITR,         &
&       elp(1), icon
  write(6,'(1A16,3I8,1F18.3,1I8)') "coremgs_swp", ndim, mvec, NITR,     &
&       elp(2), icon

  ! finalization
  deallocate( A )
  deallocate( Q )

  stop
end program main
