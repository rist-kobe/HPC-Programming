! Copyright 2024 Research Organization for Information Science and Technology
subroutine add (ns, a, b, c)
  use mytype,only: SP
  implicit none
  integer,intent(in) :: ns
  real(kind=SP),intent(out) :: a(1:ns)
  real(kind=SP),intent(in)  :: b(1:ns)
  real(kind=SP),intent(in)  :: c(1:ns)

  integer i

#if ! defined(USE_UNIFIED_MEM)
  !$acc parallel present(a,b,c)
  !$acc loop 
#else
  !$acc parallel 
  !$acc loop 
#endif
  do i = 1, ns
    a(i) = b(i) + c(i)
  end do
  !$acc end parallel
end subroutine add

subroutine saxpy (ns, a, x, y)
  use mytype,only: SP
  implicit none
  integer,intent(in) :: ns
  real(kind=SP),intent(inout)  :: y(1:ns)
  real(kind=SP),intent(in)     :: a
  real(kind=SP),intent(in)     :: x(1:ns)

  integer i

#if ! defined(USE_UNIFIED_MEM)
  !$acc parallel present(x,y)
  !$acc loop
#else
  !$acc parallel 
  !$acc loop
#endif
  do i = 1, ns
    y(i) = y(i) + a*x(i)
  end do
  !$acc end parallel 
end subroutine saxpy 

subroutine dummy (ns, a, s)
  use mytype,only: SP
  implicit none
  integer,intent(in) :: ns
  real(kind=SP),intent(in) :: a(1:ns)
  integer,intent(out) :: s
  s = 0
end subroutine
