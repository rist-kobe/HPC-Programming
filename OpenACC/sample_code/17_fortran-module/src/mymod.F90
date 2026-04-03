! Copyright 2025 Research Organization for Information Science and Technology
! Reference
! * OpenACC Getting started guid, 2.6 and 2.9;
!   https://docs.nvidia.com/hpc-sdk/compilers/openacc-gs/index.html
module mymod
  implicit none
  integer,parameter :: SP = kind(1.0)
  integer,parameter :: DP = selected_real_kind(2*precision(1.0_SP))
  
  real(kind=DP),parameter :: machine_eps = epsilon(1.0_DP)
  real(kind=DP),parameter :: delta = 1.0E-6_DP

  type vector
     integer :: nsize
     real(kind=DP),allocatable,dimension(:) :: x, y, z
  end type

  type(vector) :: s
  !$ACC declare copyin(s)

  real(kind=DP),private,allocatable,dimension(:) :: u
  !$ACC declare create(u)
  !--!$ACC declare device_resident(u) ! if allocatoin of u n host is not required.

contains
  subroutine create_u (nmax, ldevice)
    integer,intent(in) :: nmax 
    logical,intent(in) :: ldevice

    integer :: i
    allocate( u(1:nmax) ) ! allocate **host and device** memory

    !$ACC kernels if(ldevice)
    !$ACC loop
    do i = 1, nmax
       u(i) = 0.0_DP
    end do
    !$ACC end kernels
  end subroutine

  subroutine release_u ()
    deallocate( u ) ! deallocate **host and device** memory
  end subroutine

  subroutine calc_device (a, b, ns, itr) 
  !$ACC routine gang 
    real(kind=DP),contiguous,intent(inout) :: a(:)
    real(kind=DP),contiguous,intent(inout) :: b(:)
    integer,intent(in) :: ns
    integer,intent(in) :: itr

    integer :: i
    real(kind=DP) :: f

    !$ACC loop 
    do i = 1, ns 
       u(i) = u(i) + delta*(s%x(i) + s%y(i) - s%z(i))  
       f = sqrt(u(i)*u(i) + machine_eps)
       a(i) = a(i) + u(i) / f
       b(i) = b(i) - u(i) / f
    end do
  end subroutine calc_device

  subroutine calc_host (a, b, ns, itr) 
    real(kind=DP),contiguous,intent(inout) :: a(:)
    real(kind=DP),contiguous,intent(inout) :: b(:)
    integer,intent(in) :: ns
    integer,intent(in) :: itr

    integer :: i
    real(kind=DP) :: f

    do i = 1, ns 
       u(i) = u(i) + delta*(s%x(i) + s%y(i) - s%z(i))  
       f = sqrt(u(i)*u(i) + machine_eps)
       a(i) = a(i) + u(i) / f
       b(i) = b(i) - u(i) / f
    end do
  end subroutine calc_host
end module mymod
