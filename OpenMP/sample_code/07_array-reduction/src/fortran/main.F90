! Copyright 2024 Research Organization for Information Science and Technology
module commvar
  integer,parameter ::NSIZE = 8 
end module commvar

program main
  use commvar,only: NSIZE
  implicit none 
  integer :: i, j
  integer :: a(1:NSIZE)
  integer :: f(1:NSIZE,1:NSIZE)

  ![Note] collapse clause
  ! collapse(2) means the 2 successive loops can be converted
  ! into 1 single loop suitable for parallel calculations.
  !$omp parallel do schedule(static) collapse(2)
  do j = 1, NSIZE
  do i = 1, NSIZE
     f(i,j) = j - i 
  end do
  end do

  !$omp parallel do schedule(static)
  do i = 1, NSIZE
     a(i) = 0
  end do

  !$omp parallel do schedule(static) reduction(+:a)
  do j = 1, NSIZE
    do i = 1, NSIZE
       a(i) = a(i) + f(i,j)
    end do
  end do

  do i = 1, NSIZE
     write(6,'("a(",1I0,")=",1I0)') i, a(i)
  end do

  stop
end program
