! Copyright 2024 Research Organization for Information Science and Technology
program main
  use mytype,only: DP
  use mykernel,only: mmp_simple, mmp_simple_blk, mmp_lex_tp_blk, dummy
  use, intrinsic::iso_c_binding
  implicit none

! Please set NSIZE for double precision arrays satisfying the following conditions.
! (1)   8bytes*(NSIZE)^2 > (L2 cache size per core)
! (2) 3*8bytes*(NSIZE)^2 ~ (Target memory size)
! A typical choice of (Target memory size) is to set the size of 
! the memory whose latency is the highest in the memory level.
!   ex1: Main memory size (per NUMA node)
!   ex2: Size (1.2~1.5 times) larger than LLC (per NUMA node or CMG or DIE) 
#define NSIZE 1500

! Please set blocking size (for double precision array) of matrix transpose.
! You can estimate it in the unit of cache line size.
! We note that BLKSIZE_TRANS must be less than NSIZE. 
! This parameter has an impact only on mmp_lex_tp_blk
#define BLKSIZE_TRANS 40

! Please set the number of iterations in each kernel
#define NITR 30

  integer,parameter :: iu_std = 6, iou_gen = 10
  integer :: i, j
  integer :: nargc, nbk1, nbk2, nbkr, nbkc, icon
  integer :: nseed
  integer,allocatable,dimension(:) :: seed

  character(len=32) :: cbuf

  real(kind=DP),parameter :: zero = 0.0_DP, one = 1.0_DP
  real(kind=DP) :: tmp, elp0, elp, memsize
  real(kind=DP) :: mata(NSIZE,NSIZE)
  real(kind=DP) :: matb(NSIZE,NSIZE)
  real(kind=DP) :: matc(NSIZE,NSIZE)
  real(kind=DP) :: rnd(2)

  !=========================================================================
  interface
  !=========================================================================
  real(kind=c_double) function get_elp_time() bind(c)
    use,intrinsic::iso_c_binding,only: c_double
  end function get_elp_time

  real(kind=c_double) function get_cpu_time() bind(c)
    use,intrinsic::iso_c_binding,only: c_double
  end function get_cpu_time
  !=========================================================================
  end interface
  !=========================================================================

  nargc = command_argument_count()
  if ( nargc .ne. 2 ) then
     write(iu_std,'(1A,/,1A,/,1A)')                                &
&      "[usage] run.x (arg1) (arg2)",                              &
&      "        (arg1): blocking size for the innermost loop",     &
&      "        (arg2): blocking size for the 2nd innermost loop"
     stop
  end if

  memsize = (8.0_DP * NSIZE * NSIZE) / 1048576.0_DP

  write(iu_std,'("--------------------------")')
  write(iu_std,'(1A,1I5,/,1A,1F9.3)')                              &
&      " Matrix dimension (double precision; 8bytes): ", NSIZE,    &
&      " Total memory size in kernels (MiB): ", 3.0_DP*memsize

  call get_command_argument(1,cbuf)
  read(cbuf,'(1I10)') nbk1
  call get_command_argument(2,cbuf)
  read(cbuf,'(1I10)') nbk2

  call random_seed(size=nseed)
  allocate ( seed(1:nseed) )
  do i = 1, nseed
     !call system_clock(count=seed(i))
     seed(i) = 20409102 + i - 1
  end do
  call random_seed(put=seed)

  tmp = one / NSIZE
  do j = 1, NSIZE
  do i = 1, NSIZE
     call random_number(rnd)
     mata(i,j) = rnd(1) * tmp 
     matb(i,j) = rnd(2) * tmp
  end do
  end do

  deallocate( seed )

  nbkr = BLKSIZE_TRANS
  nbkc = BLKSIZE_TRANS

!=========================================================================
  write(iu_std,'(1A)') "[Mat-Mat product: Simple implementation]"

  ! warm up 
  icon = 0
  !=======================================================================
  do i = 1, 3
  !=======================================================================
  call mmp_simple(matc, mata, matb, NSIZE)
  call dummy(matc, NSIZE, NSIZE, icon) 
  !=======================================================================
  end do
  !=======================================================================

  elp0 = get_elp_time()

  icon = 0
  !=======================================================================
  do i = 1, NITR
  !=======================================================================
  call mmp_simple(matc, mata, matb, NSIZE)
  call dummy(matc, NSIZE, NSIZE, icon) 
  !=======================================================================
  end do
  !=======================================================================

  elp = get_elp_time() - elp0

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + matc(i,i)
  end do
  
  write(iu_std,                                                            & 
&   '("Elapsed (sec)",1F10.4,", # of itr. =",1I5,", trace = ",1E23.12,/)') &
&   elp, NITR, tmp
!=========================================================================
  write(iu_std,'(1A)') "[Mat-Mat product: Simple loop blocking]"

  memsize = (8.0_DP*(nbk1*nbk2 + nbk1 + nbk2))/1024.0_DP

  write(iu_std,'(1A,1F9.3)') "Estimated data size per block(KiB) ", memsize

  ! warm up
  icon = 0
  !=======================================================================
  do i = 1, 3
  !=======================================================================
  call mmp_simple_blk(matc, mata, matb, NSIZE, nbk1, nbk2)
  call dummy(matc, NSIZE, NSIZE, icon) 
  !=======================================================================
  end do
  !=======================================================================

  elp0 = get_elp_time()

  icon = 0
  !=======================================================================
  do i = 1, NITR
  !=======================================================================
  call mmp_simple_blk(matc, mata, matb, NSIZE, nbk1, nbk2)
  call dummy(matc, NSIZE, NSIZE, icon) 
  !=======================================================================
  end do
  !=======================================================================

  elp = get_elp_time() - elp0

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + matc(i,i)
  end do
  
  write(iu_std,                                                            & 
&   '("Elapsed (sec)",1F10.4,", # of itr. =",1I5,", trace = ",1E23.12,/)') &
&   elp, NITR, tmp
!=========================================================================
  write(iu_std,'(1A)') "[Mat-Mat product: Loop exchanging, &
&   Transpose, Blocking on two inner loops]"

  memsize = (8.0_DP*(nbk1*nbk2 + nbk1))/1024.0_DP

  write(iu_std,'(1A,1F9.3)') "Estimated data size per block(KiB) ", memsize

  ! warm up
  icon = 0
  !=======================================================================
  do i = 1, 3
  !=======================================================================
  call mmp_lex_tp_blk(matc, mata, matb, NSIZE, nbk1, nbk2, nbkr, nbkc)
  call dummy(matc, NSIZE, NSIZE, icon) 
  !=======================================================================
  end do
  !=======================================================================

  elp0 = get_elp_time()

  icon = 0
  !=======================================================================
  do i = 1, NITR
  !=======================================================================
  call mmp_lex_tp_blk(matc, mata, matb, NSIZE, nbk1, nbk2, nbkr, nbkc)
  call dummy(matc, NSIZE, NSIZE, icon) 
  !=======================================================================
  end do
  !=======================================================================

  elp = get_elp_time() - elp0

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + matc(i,i)
  end do
  
  write(iu_std,                                                            & 
&   '("Elapsed (sec)",1F10.4,", # of itr. =",1I5,", trace = ",1E23.12,/)') &
&   elp, NITR, tmp
!=========================================================================
  write(iu_std,'("--------------------------")')
  stop 
end program main
