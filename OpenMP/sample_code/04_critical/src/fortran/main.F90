! Copyright 2024 Research Organization for Information Science and Technology
program main
  use mymod, only: decomp_size, func, func_local
  use omp_lib
  implicit none
  integer,parameter :: NSIZE = 50
  integer :: i, ifrom, ito, tid, nt
  integer :: s
  integer :: ds
  integer :: a(NSIZE), b(NSIZE)

!$OMP parallel do schedule(static) private(i) shared(a,b)
  do i = 1, NSIZE
     a(i) = 0
     b(i) = 0
  end do

  b(1) = 3; b(2) = 0; b(3) = 2
  do i = 4, NSIZE
     b(i) = b(i-2) + b(i-3)
  end do

!$OMP parallel shared(s,a,b) private(i,ifrom,ito,tid,nt,ds)
  tid = omp_get_thread_num ()
  nt  = omp_get_num_threads ()
  
  call decomp_size(NSIZE, tid, nt, ifrom, ito) 

#if 0
  !$OMP critical (CHECKDECOMP)
  write(6,'("tid=",I0," ifrom=",I0," ito=",I0)') tid, ifrom, ito
  !$OMP end critical (CHECKDECOMP)
#endif

  !$OMP do schedule(static)
  do i = 2, NSIZE-1
     a(i) = (b(i-1) + b(i+1)) / NSIZE
  end do
  !$OMP end do

  !$OMP single
  s = 0
  !$OMP end single

#if defined(BETTER_IMPL)
  ! better implementation: func_local keeps its work array on the
  ! stack (no shared state), so only the update of the shared
  ! variable s must be protected, and atomic is enough
  ds = func_local(a,ifrom,ito)
  !$OMP atomic update
  s = s + ds
#elif ! defined(WO_CRITICAL)
  ! The module variable u is shared among the threads, so the whole
  ! call to func must be protected by a critical construct.
  !$OMP critical (FUNC)
  s = s + func(a,ifrom,ito)
  !$OMP end critical (FUNC)
#else
  ! wrong implementation
  s = s + func(a,ifrom,ito)
#endif
!$OMP end parallel

  write(6,'(1I10)') s 
end program main
