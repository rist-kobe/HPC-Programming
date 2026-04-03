! Copyright 2024 Research Organization for Information Science and Technology
program main
  use mykernel,only: saxpy_0, saxpy_1, dummy
  use mytype,only: SP, DP, NSIZE, NREP
  use timer,only: get_elp_time
  implicit none
  
  integer :: i
  integer :: s
  integer :: ielp0

  real(kind=DP) :: elp
  real(kind=SP) :: tmp
  real(kind=SP),allocatable,dimension(:) :: x
  real(kind=SP),allocatable,dimension(:) :: y

  allocate( x(1:NSIZE) )
  allocate( y(1:NSIZE) )

  ! saxpy_0
  do i = 1, NSIZE
    x(i) = 0.0001_SP
  end do

  call saxpy_0(NSIZE,3.0_SP,x,y) ! warm-up

  call get_elp_time(ielp0)

  do i = 1, NREP
    y(:) = 0.0_SP
    call saxpy_0(NSIZE,3.0_SP,x,y)
    s = s +  dummy(NSIZE,x)
  end do

  call get_elp_time(ielp0, elp)

  tmp = 0.0
  do i = 1, NSIZE
    tmp = tmp + y(i)
  end do

  write(6,'("[SAXPY_0] elp_sec:",1F10.3,1X,"check:",1E15.8)') elp, tmp

  ! saxpy_1
  do i = 1, NSIZE
    x(i) = 0.0001_SP
  end do

  call get_elp_time(ielp0)

  do i = 1, NREP
    y(:) = 0.0_SP
    call saxpy_1(NSIZE,3.0_SP,x,y)
    s = s +  dummy(NSIZE,x)
  end do

  call get_elp_time(ielp0, elp)

  tmp = 0.0
  do i = 1, NSIZE
    tmp = tmp + y(i)
  end do

  write(6,'("[SAXPY_1] elp_sec:",1F10.3,1X,"check:",1E15.8)') elp, tmp

  deallocate ( x )
  deallocate ( y )
  stop
end program main
