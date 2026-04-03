! Copyright 2025 Research Organization for Information Science and Technology
program main
  implicit none
  integer,parameter :: n = 64
  integer :: i
  real,allocatable,dimension(:) :: a, b

  allocate( a(1:n) )
  allocate( b(1:n) )

  do i = 1, n
    a(i) = i*1.0
    b(i) = i*10.0
  end do

  !$acc kernels 
  !$acc loop
  do i = 1, n
    a(i) = a(i) + 2.0
    b(i) = b(i) * 2.0
  end do
  !$acc end kernels

  write(6,'(2F)') a(:), b(:)

  stop
end program main
