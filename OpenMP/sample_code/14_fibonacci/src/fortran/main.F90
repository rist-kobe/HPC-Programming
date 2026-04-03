! Copyright 2024 Research Organization for Information Science and Technology
!-----------------------------------------------------
! Fibonacci series
! The original code is from Fig.7.11 in 
! T. G. Mattson, Y. (Helen) He, and A. E. Kniges,
! The OpenMP Common core: Making OpenMP Simple Again
! (MIT,2019)
! http://www.ompcore.com
!-----------------------------------------------------
recursive integer function fib_omp (n) result (ret)
  implicit none
  integer,intent(in) :: n

  integer :: x, y
  if ( n .lt. 2 ) then
     ret = n
     return
  end if
  !$omp task shared(x)
  x = fib_omp (n-1)
  !$omp end task
  !$omp task shared(y)
  y = fib_omp (n-2)
  !$omp end task
  !$omp taskwait
  ret = x + y
  return
end function fib_omp

program main
  implicit none
  integer :: i, vn
  interface
  recursive integer function fib_omp (n) 
    implicit none
    integer,intent(in) :: n
  end function fib_omp
  end interface
  do i = 1, 10
    !$omp parallel
    !$omp single
    vn = fib_omp(i)
    !$omp end single
    !$omp end parallel
    write(6,'("fibonacci_",1I0,"=",1I3)') i,vn
  end do
  stop
end program main
