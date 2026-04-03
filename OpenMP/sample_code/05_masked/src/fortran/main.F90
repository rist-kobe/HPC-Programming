! Copyright 2024 Research Organization for Information Science and Technology 
!======================================================================
! Note: masked construct is introduced in OpenMP 5.1. 
!       This is regarded as an extension of master construct.          
!       Please check your OpenMP version and the supported features 
!       before compiling the program.                                   
!       If your compiler support masked construct, please set 
!       -DNOT_USE_MAKSED in the compiler's option.                  
!======================================================================
program main
  use omp_lib
  implicit none

! x86 (Intel, AMD, etc.): 64-bytes                                     
#define CACHE_LINE_DP   8
#define CACHE_LINE_INT 16

! aarch64: 256 bytes  
!#define CACHE_LINE_DP   64
!#define CACHE_LINE_INT 128 

  integer,allocatable,dimension(:) :: a
  integer :: nt, tid, id

  nt = omp_get_max_threads()

!$omp parallel shared(a) private(tid, id)
  tid = omp_get_thread_num()

#if ! defined(NOT_USE_MASKED)
  !$omp masked
     ! To avoid false sharing
     allocate( a(1:nt*CACHE_LINE_INT) )
  !$omp end masked
#else
  if ( tid .eq. 0 ) then
     ! To avoid false sharing
     allocate( a(1:nt*CACHE_LINE_INT) )
  end if
#endif

  !$omp barrier

  id = tid*CACHE_LINE_INT + 1;

  a(id) = 0;

#if ! defined(NOT_USE_MASKED)
  !$omp masked filter(1)
     a(id) = 1
  !$omp end masked
#else
  if ( tid .eq. 1 ) then
     a(id) = 1
  end if
#endif

  a(id) = a(id) + 1
!$omp end parallel

  do tid = 0, nt-1
    id = tid*CACHE_LINE_INT + 1
    write(6,'("Thread ",1I5,": a(",1I5,") = ",1I10)') tid, id, a(id)
  end do

  deallocate( a )

  stop
end program main
