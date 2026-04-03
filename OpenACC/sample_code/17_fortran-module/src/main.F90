! Copyright 2025 Research Organization for Information Science and Technology
! Reference
! * OpenACC Getting started guid, 2.6 and 2.9;
!   https://docs.nvidia.com/hpc-sdk/compilers/openacc-gs/index.html
program main
  use mymod
  use omp_lib
  implicit none
  integer :: nsize, ierr, nitr 
  integer :: i, itr
  logical :: ldev
  character(len=32) :: cbuf
  real(kind=DP) :: elp, elp0, elp1, elp2
  real(kind=DP) :: chk1, chk2
  real(kind=DP),allocatable,dimension(:) :: a, b

  ierr = command_argument_count ()
  if ( ierr .ne. 2 ) then
     write (6,'(1a32)') "[usage] run.x $1 $2"
     write (6,'(1a32)') "   $1: vector size (integer)"
     write (6,'(1a32)') "   $2: number of iteration (integer)"
     stop
  end if

  call get_command_argument (number=1, value=cbuf)
  read (cbuf,'(1I10)') nsize
  if ( nsize .le. 0 ) then
     nsize = 10000 
  end if

  call get_command_argument (number=2, value=cbuf)
  read (cbuf,'(1I10)') nitr 
  if ( nitr .le. 0 ) then
     nitr = 5000 
  end if

  s%nsize = nsize
  allocate( s%x(1:nsize) )
  allocate( s%y(1:nsize) )
  allocate( s%z(1:nsize) )

  allocate( a(1:nsize) )
  allocate( b(1:nsize) )

  do i = 1, nsize
    s%x(i) = 1.0_DP
    s%y(i) = 1.0_DP
    s%z(i) = 1.0_DP
  end do

  do i = 1, nsize
    a(i) = 1.0_DP
    b(i) = 2.0_DP
  end do

  write (6,'("type",1x,"nsize",1x,"nitr",1x,"elp_sec_kernel",1x,"elp_sec_total")')

  !---------------------------------------------------------------------
  ! Run calc in device
  !---------------------------------------------------------------------
  ldev = .true.
  call create_u ( nsize, ldev ) ! work array in devive is created.

  elp0 = omp_get_wtime()

  !$ACC enter data copyin(a(1:nsize),b(1:nsize))  &
  !$ACC & copyin(s%x(1:nsize),s%y(1:nsize),s%z(1:nsize))

  elp1 = omp_get_wtime()

  do itr = 1, nitr
     !$ACC parallel present(a(1:nsize),b(1:nsize)) 
     call calc_device(a, b, nsize, itr)
     !$ACC end parallel
  end do

  elp2 = omp_get_wtime() - elp1

  !$ACC exit data copyout(a(1:nsize),b(1:nsize)) 

  elp = omp_get_wtime() - elp0

  call release_u () ! work array in device is deleted.

  write (6,'("device",1I10,1I10,1x,1F13.4,1F13.4)') nsize,nitr,elp2,elp

  chk1 = a(1)  
  chk2 = b(1)  

  do i = 1, nsize
    a(i) = 1.0_DP
    b(i) = 2.0_DP
  end do

  !---------------------------------------------------------------------
  ! Run calc in host
  !---------------------------------------------------------------------
  ldev = .false.
  call create_u ( nsize, ldev ) ! work array in host is created.

  elp0 = omp_get_wtime()

  do itr = 1, nitr
     call calc_host(a, b, nsize, itr)
  end do

  elp = omp_get_wtime() - elp0

  call release_u () ! work array in host is deleted.

  !write (6,'(E20.6,E20.6)') a(1), b(1)

  chk1 = a(1) - chk1 
  chk2 = b(1) - chk2

  elp2 = elp
  write (6,'("host  ",1I10,1I10,1x,1F13.4,1F13.4)') nsize,nitr,elp2,elp
  write (6,'("Diff device and host",2E22.8)') chk1, chk2

  stop
end program main
