! Copyright 2024 Research Organization for Information Science and Technology
program main
  use mytype,only: DP, NSIZE, NREP
  use omp_lib
  implicit none
  
  integer :: i
  integer :: s

  real(kind=DP) :: elp
  real(kind=DP) :: elp0
  real(kind=DP) :: tmp
  real(kind=DP) :: a(1:NSIZE)
  real(kind=DP) :: b(1:NSIZE)
  real(kind=DP) :: c(1:NSIZE)

  external add, dummy

  !$omp parallel do
  do i = 1, NSIZE
    a(i) = 0.0
    b(i) = 1.0
    c(i) = 2.0
  end do
  !$omp end parallel do

  elp0 = omp_get_wtime()

  do i = 1, NREP
    call add(NSIZE,a,b,c)
    call dummy(NSIZE,a,s)
    s = s + i
  end do

  elp = omp_get_wtime() - elp0

  tmp = 0.0
  do i = 1, NSIZE
    tmp = tmp + a(i)
  end do

  write(6,'("elp_sec:",1F10.3,1X,"check:",1E15.8)') elp, tmp

  stop
end program main
