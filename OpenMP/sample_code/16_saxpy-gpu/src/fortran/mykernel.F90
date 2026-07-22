! Copyright 2024 Research Organization for Information Science and Technology
!======================================================================
module mykernel
!======================================================================
  use mytype,only: SP
  implicit none

contains
!======================================================================
subroutine saxpy_0 (ns, a, x, y)
!======================================================================
  integer,intent(in) :: ns
  real(kind=SP),contiguous,intent(inout)  :: y(:)
  real(kind=SP),intent(in)                :: a
  real(kind=SP),contiguous,intent(in)     :: x(:)

  integer i

#if _OPENACC
  !$ACC kernels
  !$ACC loop
#endif
#if _OPENMP
  !$OMP target
  !$OMP loop
#endif
  do i = 1, ns
    y(i) = y(i) + a*x(i)
  end do
#if _OPENACC
  !$ACC end kernels
#endif
#if _OPENMP
  !$OMP end target
#endif
!======================================================================
end subroutine saxpy_0
!======================================================================

!======================================================================
subroutine saxpy_1 (ns, a, x, y)
!======================================================================
  integer,intent(in) :: ns
  real(kind=SP),contiguous,intent(inout)  :: y(:)
  real(kind=SP),intent(in)                :: a
  real(kind=SP),contiguous,intent(in)     :: x(:)

  integer i

#if _OPENACC
  !$ACC kernels copy(y(1:ns)) copyin(x(1:ns))
  !$ACC loop
#endif
#if _OPENMP
  !$OMP target map(tofrom:y(1:ns)) map(to:x(1:ns))
  !$OMP loop
#endif
  do i = 1, ns
    y(i) = y(i) + a*x(i)
  end do
#if _OPENACC
  !$ACC end kernels
#endif
#if _OPENMP
  !$OMP end target
#endif
!======================================================================
end subroutine saxpy_1
!======================================================================

!======================================================================
subroutine saxpy_2 (ns, a, x, y)
!======================================================================
  ! x is expected to be device-resident already (enclosing data
  ! region), so only y is mapped per call.
  integer,intent(in) :: ns
  real(kind=SP),contiguous,intent(inout)  :: y(:)
  real(kind=SP),intent(in)                :: a
  real(kind=SP),contiguous,intent(in)     :: x(:)

  integer i

#if _OPENACC
  !$ACC kernels copy(y(1:ns)) present(x(1:ns))
  !$ACC loop
#endif
#if _OPENMP
  ! x is not mapped here: the enclosing target data region keeps it
  ! device-resident, so the implicit reference reuses that copy.
  !$OMP target map(tofrom:y(1:ns))
  !$OMP loop
#endif
  do i = 1, ns
    y(i) = y(i) + a*x(i)
  end do
#if _OPENACC
  !$ACC end kernels
#endif
#if _OPENMP
  !$OMP end target
#endif
!======================================================================
end subroutine saxpy_2
!======================================================================

!======================================================================
function dummy (ns, a) result(s)
!======================================================================
  integer,intent(in) :: ns
  real(kind=SP),intent(in) :: a(:)
  integer :: s
  s = 0
!======================================================================
end function dummy
!======================================================================

!======================================================================
end module mykernel
!======================================================================
