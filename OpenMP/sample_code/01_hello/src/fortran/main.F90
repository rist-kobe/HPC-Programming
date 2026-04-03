! Copyright 2024 Research Organization for Information Science and Technology
program main
#if defined(_OPENMP)
  use omp_lib
#endif
  integer :: tid
  integer :: nthreads

#if defined(_OPENMP)
  nthreads = omp_get_max_threads()
#else
  nthreads = 1
  tid = 0
#endif

  write(6,'("Hello")') 
  !$omp parallel private(tid)
#if defined(_OPENMP)
  tid = omp_get_thread_num()
#endif
! Use of omp critical is correct. Why?
  !$omp critical
  write (6,'("Hello. I am thread-ID ",1I3," of ",1I3,&
&        " threads.")') tid, nthreads
  !$omp end critical
! Naive implementation
!  write (6,'("Hello. I am thread-ID ",1I3," of ",1I3,&
! &        " threads.")') tid, nthreads
  !$omp end parallel

  stop
end program main
