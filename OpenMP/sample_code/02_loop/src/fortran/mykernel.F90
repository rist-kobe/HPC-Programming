! Copyright 2024 Research Organization for Information Science and Technology
subroutine add (ns, a, b, c)
  use mytype,only: DP
  implicit none
  integer,intent(in) :: ns
  real(kind=DP),intent(out) :: a(1:ns)
  real(kind=DP),intent(in)  :: b(1:ns)
  real(kind=DP),intent(in)  :: c(1:ns)

  integer i

  !$omp parallel do private(i) &
  !$omp & shared(a,b,c) schedule(static)
  do i = 1, ns
    a(i) = b(i) + c(i)
  end do
  !$omp end parallel do
end subroutine add

subroutine daxpy (ns, a, x, y)
  use mytype,only: DP
  implicit none
  integer,intent(in) :: ns
  real(kind=DP),intent(inout)  :: y(1:ns)
  real(kind=DP),intent(in)     :: a
  real(kind=DP),intent(in)     :: x(1:ns)

  integer i

  do i = 1, ns
    y(i) = y(i) + a*x(i)
  end do
end subroutine daxpy 

subroutine dummy (ns, a, s)
  use mytype,only: DP
  implicit none
  integer,intent(in) :: ns
  real(kind=DP),intent(in) :: a(1:ns)
  integer,intent(out) :: s
  s = 0
end subroutine
