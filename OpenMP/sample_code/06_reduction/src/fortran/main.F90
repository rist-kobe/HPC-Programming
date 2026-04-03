! Copyright 2024 Research Organization for Information Science and Technology
program main
   implicit none
   integer :: a(1:100)
   integer :: amax
   integer :: i,nthds

   nthds = 1
   do i = 1, 100
      a(i) = 0
   end do
   a(1) = 1

   amax = 0
   !$omp parallel do reduction(max:amax)
   do i = 1, 100
      amax = max(amax,a(i))
   end do
   write(6,'("Max val of array a: ",1I0)') amax 
   stop
end program main
