! Copyright 2025 Research Organization for Information Science and Technology
subroutine saxpy_0 (ns, a, x, y)
  use mytype,only: SP
  implicit none
  integer,intent(in) :: ns
  real(kind=SP),intent(inout)  :: y(1:ns)
  real(kind=SP),intent(in)     :: a
  real(kind=SP),intent(in)     :: x(1:ns)

  integer i

  !$acc kernels
  do i = 1, ns
    y(i) = y(i) + a*x(i)
  end do
  !$acc end kernels
end subroutine saxpy_0

subroutine saxpy_omp_0 (ns, a, x, y)
  use mytype,only: SP
  implicit none
  integer,intent(in) :: ns
  real(kind=SP),intent(inout)  :: y(1:ns)
  real(kind=SP),intent(in)     :: a
  real(kind=SP),intent(in)     :: x(1:ns)

  integer i

  !$omp parallel do schedule(static)
  do i = 1, ns
    y(i) = y(i) + a*x(i)
  end do
  !$omp end parallel do
end subroutine saxpy_omp_0

subroutine saxpy_1 (ns, a, x, y)
  use mytype,only: SP
  implicit none
  integer,intent(in) :: ns
  real(kind=SP),intent(inout)  :: y(1:ns)
  real(kind=SP),intent(in)     :: a
  real(kind=SP),intent(in)     :: x(1:ns)

  integer i

  !$acc data copy(y(1:ns)) copyin(x(1:ns))
  !$acc kernels
  do i = 1, ns
    y(i) = y(i) + a*x(i)
  end do
  !$acc end kernels
  !$acc end data
end subroutine saxpy_1

subroutine dummy (ns, a, s)
  use mytype,only: SP
  implicit none
  integer,intent(in) :: ns
  real(kind=SP),intent(in) :: a(1:ns)
  integer,intent(out) :: s
  s = 0
end subroutine
