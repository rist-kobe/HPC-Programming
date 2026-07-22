! Copyright 2024 Research Organization for Information Science and Technology 
!======================================================================
! Note: masked construct is introduced in OpenMP 5.1. 
!       This is regarded as an extension of master construct: masked
!       without a filter clause behaves like filter(0), i.e., only the
!       primary thread executes the region. It is the OpenMP 5.1
!       replacement for the deprecated master construct.
!       No implied barrier exists at entry to or exit from a masked
!       region; that is why an explicit barrier is placed after the
!       allocation below.
!       Please check your OpenMP version and the supported features 
!       before compiling the program.                                   
!       If your compiler does NOT support masked construct, please set 
!       -DNOT_USE_MASKED in the compiler's option.                  
!======================================================================
program main
  use omp_lib
  implicit none

! Number of array elements per cache line. CACHE_LINE_INT is used for
! the integer work array below; CACHE_LINE_DP (double precision) is
! provided for the exercises.
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
     ! To avoid false sharing: each thread works on its own cache line
     allocate( a(1:nt*CACHE_LINE_INT) )
  !$omp end masked
#else
  if ( tid .eq. 0 ) then
     ! To avoid false sharing: each thread works on its own cache line
     allocate( a(1:nt*CACHE_LINE_INT) )
  end if
#endif

  ! masked has no implied barrier: all threads must wait here
  ! until the allocation by thread 0 is visible.
  !$omp barrier

  id = tid*CACHE_LINE_INT + 1

  a(id) = 0

#if ! defined(NOT_USE_MASKED)
  ! Only the thread whose number equals the filter integer runs
  ! this region. With a single thread no thread matches filter(1)
  ! and the region is simply skipped.
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
end program main
