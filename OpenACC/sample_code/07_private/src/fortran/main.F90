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
  real(kind=SP),allocatable,dimension(:) :: a

  external func_0, func_1, func_2, func_3, dummy

  allocate( a(1:NSIZE) )

  ! warm up
  do i = 1, NSIZE
    a(i) = 1.0_SP
  end do

  call func_3(NSIZE, a)

  ! func_0
  do i = 1, NSIZE
    a(i) = 1.0_SP
  end do

  elp0 = omp_get_wtime()

  do i = 1, NREP
    call func_0(NSIZE,a)
    call dummy(NSIZE,a,s)
    s = s + i
    do i = 1, NSIZE
      a(i) = 1.0_SP
    end do
  end do

  elp = omp_get_wtime() - elp0

  tmp = 0.0
  do i = 1, NSIZE
    tmp = tmp + a(i)
  end do

  write(6,'("[FUNC_0] elp_sec:",1F10.3,1X,"check:",1E15.8)') elp, tmp

  ! func_1
  do i = 1, NSIZE
    a(i) = 1.0_SP
  end do

  elp0 = omp_get_wtime()

  do i = 1, NREP
    call func_1(NSIZE,a)
    call dummy(NSIZE,a,s)
    s = s + i
    do i = 1, NSIZE
      a(i) = 1.0_SP
    end do
  end do

  elp = omp_get_wtime() - elp0

  tmp = 0.0
  do i = 1, NSIZE
    tmp = tmp + a(i)
  end do

  write(6,'("[FUNC_1] elp_sec:",1F10.3,1X,"check:",1E15.8)') elp, tmp

  ! func_2
  do i = 1, NSIZE
    a(i) = 1.0_SP
  end do

  elp0 = omp_get_wtime()

  do i = 1, NREP
    call func_2(NSIZE,a)
    call dummy(NSIZE,a,s)
    s = s + i
    do i = 1, NSIZE
      a(i) = 1.0_SP
    end do
  end do

  elp = omp_get_wtime() - elp0

  tmp = 0.0
  do i = 1, NSIZE
    tmp = tmp + a(i)
  end do

  write(6,'("[FUNC_2] elp_sec:",1F10.3,1X,"check:",1E15.8)') elp, tmp

  ! func_3
  do i = 1, NSIZE
    a(i) = 1.0_SP
  end do

  elp0 = omp_get_wtime()

  do i = 1, NREP
    call func_3(NSIZE,a)
    call dummy(NSIZE,a,s)
    s = s + i
    do i = 1, NSIZE
      a(i) = 1.0_SP
    end do
  end do

  elp = omp_get_wtime() - elp0

  tmp = 0.0
  do i = 1, NSIZE
    tmp = tmp + a(i)
  end do

  write(6,'("[FUNC_3] elp_sec:",1F10.3,1X,"check:",1E15.8)') elp, tmp

  deallocate ( a )
  stop
end program main
