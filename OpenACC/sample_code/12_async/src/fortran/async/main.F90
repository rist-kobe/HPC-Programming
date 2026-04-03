! Copyright 2024 Research Organization for Information Science and Technology 
program main
#if defined(USE_NVTX)
  use nvtx
#endif
  implicit none

  integer,parameter :: NGSIZE = 2048
  integer,parameter :: NCSIZE = 200
  integer,parameter :: NREP = 1000

  integer :: i, j, k, itr
  real :: tmp
  real,allocatable,dimension(:,:) :: a, b, c
  real,allocatable,dimension(:,:) :: a2, b2, c2

  allocate( a(1:NGSIZE,1:NGSIZE) )
  allocate( b(1:NGSIZE,1:NGSIZE) )
  allocate( c(1:NGSIZE,1:NGSIZE) )
  allocate( a2(1:NCSIZE,1:NCSIZE) )
  allocate( b2(1:NCSIZE,1:NCSIZE) )
  allocate( c2(1:NCSIZE,1:NCSIZE) )

  do j = 1, NGSIZE
  do i = 1, NGSIZE
     a(i,j) = 0.01
     b(i,j) = 0.02
  end do
  end do
 
  do j = 1, NCSIZE
  do i = 1, NCSIZE
     a2(i,j) = 0.01
     b2(i,j) = 0.02
  end do
  end do

  ! main loop
#if defined(USE_NVTX)
  call nvtxStartRange("total")
#endif
!----------------------------------------------------------------------
  !$ACC data copyin(a(1:NGSIZE,1:NGSIZE),b(1:NGSIZE,1:NGSIZE)) &
  !$ACC & copyout(c(1:NGSIZE,1:NGSIZE))
!----------------------------------------------------------------------
  itrloop: do itr = 1, NREP
     #if defined(USE_NVTX)
     call nvtxStartRange("gpu_acc")
     #endif
     !$ACC kernels async(1)
     !$ACC loop collapse(2)
     do j = 1, NGSIZE
     do i = 1, NGSIZE
        c(i,j) = 0.0
     end do
     end do

     !$ACC loop private(tmp)
     do j = 1, NGSIZE
     do i = 1, NGSIZE
        tmp = 0.0
        !$ACC loop seq
        do k = 1, NGSIZE
           tmp = tmp + a(i,k)*b(k,j)
        end do
        c(i,j) = tmp
     end do
     end do
     !$ACC end kernels

     #if defined(USE_NVTX)
     call nvtxStartRange("cpu_serial")
     #endif
     do j = 1, NCSIZE
     do i = 1, NCSIZE
        c2(i,j) = 0.0
     end do
     end do

     do j = 1, NCSIZE
     do i = 1, NCSIZE
        tmp = 0.0
        do k = 1, NCSIZE
           tmp = tmp + a2(i,k)*b2(k,j)
        end do
        c2(i,j) = tmp
     end do
     end do
     #if defined(USE_NVTX)
     call nvtxEndRange() ! cpu_serial
     #endif
     !$ACC wait(1)
     #if defined(USE_NVTX)
     call nvtxEndRange() ! gpu_acc
     #endif
  end do itrloop
!----------------------------------------------------------------------
  !$ACC end data
!----------------------------------------------------------------------
#if defined(USE_NVTX)
  call nvtxEndRange() ! total
#endif

  ! dummy output
  write(6,'(2F15.6)') c(1,1), c2(1,1)

  deallocate( a )
  deallocate( b )
  deallocate( c )
  deallocate( a2 )
  deallocate( b2 )
  deallocate( c2 )

  stop
end program main
