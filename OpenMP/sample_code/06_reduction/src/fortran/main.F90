! Copyright 2024 Research Organization for Information Science and Technology
program main
   implicit none
   integer, parameter :: n = 100
   integer :: a(1:n)
   integer :: amax
   integer :: i

   a = 0
   a(1) = 1

   ! -huge(amax) is the identity value of a max reduction: it is
   ! correct even when every element of the array is negative. OpenMP
   ! itself initializes each thread's private copy of amax with the
   ! minimum representable value and combines the private results at
   ! the end of the loop.
   amax = -huge(amax)
   !$omp parallel do reduction(max:amax)
   do i = 1, n
      amax = max(amax,a(i))
   end do
   write(6,'("Max val of array a: ",1I0)') amax 
end program main
