! Copyright 2024 Research Organization for Information Science and Technology
module commvar
  integer,parameter ::NSIZE = 8 
end module commvar

program main
  use openacc
  use commvar,only: NSIZE
  implicit none 
  integer :: i, j
  integer :: ng
  integer(kind=acc_device_kind) :: dev_type
  integer,allocatable,dimension(:) :: a
  integer,allocatable,dimension(:,:) :: f

  dev_type = acc_get_device_type()
  ng = acc_get_num_devices(dev_type)
  if ( ng .eq. 0 ) then
     write(6,'("There is no available device.")')
     stop
  end if
  write(6,'("Number of OpenACC capable devices : ",1I03)') ng

  allocate( a(1:NSIZE) )
  allocate( f(1:NSIZE,1:NSIZE) )

!----------------------------------------------------------------------
  !$acc data create(f(1:NSIZE,1:NSIZE)) copyout(a(1:NSIZE))
!----------------------------------------------------------------------
  !$acc kernels 
  !$acc loop independent collapse(2)
  do j = 1, NSIZE
  do i = 1, NSIZE
     f(i,j) = j - i 
  end do
  end do
  !$acc end kernels

  !$acc kernels 
  !$acc loop independent 
  do i = 1, NSIZE
     a(i) = 0
  end do
  !$acc end kernels

  !$acc parallel reduction(+:a(1:NSIZE)) 
  !$acc loop 
  do j = 1, NSIZE
    do i = 1, NSIZE
       a(i) = a(i) + f(i,j)
    end do
  end do
  !$acc end parallel
!----------------------------------------------------------------------
  !$acc end data 
!----------------------------------------------------------------------

  do i = 1, NSIZE
     write(6,'("a(",1I0,")=",1I0)') i, a(i)
  end do

  stop
end program
