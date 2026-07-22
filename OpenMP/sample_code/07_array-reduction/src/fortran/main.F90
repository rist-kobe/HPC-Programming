! Copyright 2024 Research Organization for Information Science and Technology
program main
  implicit none 
  integer, parameter :: NSIZE = 8
  integer :: i, j
  integer :: a(1:NSIZE)
  integer :: f(1:NSIZE,1:NSIZE)

  ![Note] collapse clause
  ! collapse(2) means the 2 successive loops can be converted
  ! into 1 single loop suitable for parallel calculations.
  !$omp parallel do schedule(static) collapse(2)
  do j = 1, NSIZE
  do i = 1, NSIZE
     f(i,j) = i - j 
  end do
  end do

  !$omp parallel do schedule(static)
  do i = 1, NSIZE
     a(i) = 0
  end do

  ! Each thread gets a private copy of the whole array a, initialized
  ! to 0; OpenMP combines the private copies element-wise into the
  ! original array at the end of the loop. The parallel loop runs over
  ! the second index j, so every thread accumulates into every a(i)
  ! and the array reduction is essential.
  ! Note: Fortran supports whole-array reduction since OpenMP 4.5,
  ! whereas C needed array sections until OpenMP 5.0.
  !$omp parallel do schedule(static) reduction(+:a)
  do j = 1, NSIZE
    do i = 1, NSIZE
       a(i) = a(i) + f(i,j)
    end do
  end do

  do i = 1, NSIZE
     write(6,'("a(",1I0,")=",1I0)') i, a(i)
  end do

end program
