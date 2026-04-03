! Copyright 2024 Research Organization for Information Science and Technology
module commvar
  integer,parameter ::NSIZE = 8 
end module commvar

program main
  use omp_lib
  use commvar,only: NSIZE
  implicit none 
  integer :: i, j
  integer :: ng
  integer,allocatable,dimension(:) :: a
  integer,allocatable,dimension(:,:) :: f

  ng = omp_get_num_devices() 

  if ( ng .eq. 0 ) then
     write(6,'("There is no available device.")')
     stop
  else  
     write(6,'(1I03," OpenMP Target Capable devices are detected.")') ng
  end if

  allocate( a(1:NSIZE) )
  allocate( f(1:NSIZE,1:NSIZE) )

!----------------------------------------------------------------------
  !$omp target data map(alloc:f(1:NSIZE,1:NSIZE)) map(from:a(1:NSIZE))
!----------------------------------------------------------------------
  !$omp target 
  !$omp loop collapse(2)
  do j = 1, NSIZE
  do i = 1, NSIZE
     f(i,j) = j - i 
  end do
  end do
  !$omp end loop
  !$omp end target

  !$omp target 
  !$omp loop 
  do i = 1, NSIZE
     a(i) = 0
  end do
  !$omp end loop
  !$omp end target

  !$omp target 
  !$omp loop reduction(+:a(1:NSIZE))
  do j = 1, NSIZE
    do i = 1, NSIZE
       a(i) = a(i) + f(i,j)
    end do
  end do
  !$omp end loop
  !$omp end target
!----------------------------------------------------------------------
  !$omp end target data 
!----------------------------------------------------------------------

  do i = 1, NSIZE
     write(6,'("a(",1I0,")=",1I0)') i, a(i)
  end do

  stop
end program
