! Copyright 2024 Research Organization for Information Science and Technology
subroutine func_0 (ns, A)
  use mytype,only: SP
  implicit none
  integer,intent(in) :: ns
  real(kind=SP),intent(inout)  :: A(1:ns)

  integer :: i, ii
  real(kind=SP) :: s
  real(kind=SP) :: tmp(16)

  do i = 1, ns
     do ii = 1, 16
        tmp(ii) = mod(i+ii, 2)
     end do
     s = 0.0_SP
     do ii = 1, 16
        s = s + tmp(ii)*A(i)
     end do
     A(i) = s
  end do
end subroutine func_0

subroutine func_1 (ns, A)
  use mytype,only: SP
  implicit none
  integer,intent(in) :: ns
  real(kind=SP),intent(inout)  :: A(1:ns)

  integer :: i, ii
  real(kind=SP) :: s
  real(kind=SP) :: tmp(16)

  !$acc kernels
  !$acc loop
  do i = 1, ns
     do ii = 1, 16
        tmp(ii) = mod(i+ii, 2)
     end do
     s = 0.0_SP
     do ii = 1, 16
        s = s + tmp(ii)*A(i)
     end do
     A(i) = s
  end do
  !$acc end kernels
end subroutine func_1

subroutine func_2 (ns, A)
  use mytype,only: SP
  implicit none
  integer,intent(in) :: ns
  real(kind=SP),intent(inout)  :: A(1:ns)

  integer :: i, ii
  real(kind=SP) :: s
  real(kind=SP) :: tmp(16) ! tmp(:) means tmp(1:16)

  !$acc kernels
  !$acc loop private(tmp(:),s)
  do i = 1, ns
     do ii = 1, 16
        tmp(ii) = mod(i+ii, 2)
     end do
     s = 0.0_SP
     do ii = 1, 16
        s = s + tmp(ii)*A(i)
     end do
     A(i) = s
  end do
  !$acc end kernels
end subroutine func_2

subroutine func_3 (ns, A)
  use mytype,only: SP
  implicit none
  integer,intent(in) :: ns
  real(kind=SP),intent(inout)  :: A(1:ns)

  integer :: i, ii
  real(kind=SP) :: s
  real(kind=SP) :: tmp(16) ! tmp(:) means tmp(1:16)

  !$acc data copy(A(1:ns)) create(tmp(1:16))
  !$acc kernels
  !$acc loop private(tmp,s)
  do i = 1, ns
     !$acc loop 
     do ii = 1, 16
        tmp(ii) = mod(i+ii, 2)
     end do
     s = 0.0_SP
     !$acc loop reduction(+:s)
     do ii = 1, 16
        s = s + tmp(ii)*A(i)
     end do
     A(i) = s
  end do
  !$acc end kernels
  !$acc end data 
end subroutine func_3

subroutine dummy (ns, a, s)
  use mytype,only: SP
  implicit none
  integer,intent(in) :: ns
  real(kind=SP),intent(in) :: a(1:ns)
  integer,intent(out) :: s
  s = 0
end subroutine
