! Copyright 2024 Research Organization for Information Science and Technology
program main
  use mytype,only: SP, DP, NSIZE, NREP
  use omp_lib
  implicit none
  
  integer :: i
  integer :: s

  real(kind=DP) :: elp
  real(kind=DP) :: elp0
  real(kind=SP) :: tmp
  real(kind=SP) :: a(1:NSIZE)
  real(kind=SP) :: b(1:NSIZE)
  real(kind=SP) :: c(1:NSIZE)

  external add, saxpy, dummy

  ! add
  do i = 1, NSIZE
    a(i) = 0.0
    b(i) = 1.0 
    c(i) = 2.0
  end do

  elp0 = omp_get_wtime()

#if ! defined(USE_UNIFIED_MEM)
  !$acc data copyin(b(1:NSIZE),c(1:NSIZE)) copy(a(1:NSIZE))
#endif
  do i = 1, NREP
    call add(NSIZE,a,b,c)
    call dummy(NSIZE,a,s)
    s = s + i
  end do
#if ! defined(USE_UNIFIED_MEM)
  !$acc end data
#endif

  elp = omp_get_wtime() - elp0

  write(6,'("[ADD] elp_sec:",1F10.3,1X)') elp

  ! saxpy
  do i = 1, NSIZE
    a(i) = 0.0
    b(i) = 1.0 
  end do

  tmp = 1.0

  elp0 = omp_get_wtime()

#if ! defined(USE_UNIFIED_MEM)
  !$acc data copyin(b(1:NSIZE)) copy(a(1:NSIZE))
#endif
  do i = 1, NREP
    call saxpy(NSIZE,tmp,b,a)
    call dummy(NSIZE,a,s)
    s = s + i
  end do
#if ! defined(USE_UNIFIED_MEM)
  !$acc end data
#endif

  elp = omp_get_wtime() - elp0

  write(6,'("[SAXPY] elp_sec:",1F10.3,1X)') elp

  stop
end program main
