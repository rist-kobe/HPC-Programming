! Copyright 2024 Research Organization for Information Science and Technology
module mykernel
  use mytype, only: DP
  implicit none
contains
  subroutine poly1(ndim, x, c1, c2)
     integer, intent(in) :: ndim
     real(kind=DP), intent(inout) :: x(ndim)
     real(kind=DP), intent(in) :: c1(ndim)
     real(kind=DP), intent(in) :: c2(ndim)
     
     integer :: i
     forall(i=1:ndim) x(i) = c1(i) + x(i)*c2(i)
  end subroutine poly1

  subroutine poly4(ndim, x, c1, c2)
     integer, intent(in) :: ndim
     real(kind=DP), intent(inout) :: x(ndim)
     real(kind=DP), intent(in) :: c1(ndim)
     real(kind=DP), intent(in) :: c2(ndim)
     
     integer :: i
     forall(i=1:ndim) x(i) = c1(i)                           &
        +x(i)*(c2(i)+x(i)*(c1(i)+x(i)*(c2(i)+x(i)*c1(i)      &
        )))
  end subroutine poly4

  subroutine poly8(ndim, x, c1, c2)
     integer, intent(in) :: ndim
     real(kind=DP), intent(inout) :: x(ndim)
     real(kind=DP), intent(in) :: c1(ndim)
     real(kind=DP), intent(in) :: c2(ndim)
     
     integer :: i
     forall(i=1:ndim) x(i) = c1(i)                           &
        +x(i)*(c2(i)+x(i)*(c1(i)+x(i)*(c2(i)+x(i)*(c1(i)     &
        +x(i)*(c2(i)+x(i)*(c1(i)+x(i)*(c2(i)+x(i)*c1(i)      &
        )))))))
  end subroutine poly8

  subroutine poly16(ndim, x, c1, c2)
     integer, intent(in) :: ndim
     real(kind=DP), intent(inout) :: x(ndim)
     real(kind=DP), intent(in) :: c1(ndim)
     real(kind=DP), intent(in) :: c2(ndim)
     
     integer :: i
     forall(i=1:ndim) x(i) = c1(i)                           &
        +x(i)*(c2(i) +x(i)*(c1(i) +x(i)*(c2(i) +x(i)*( c1(i) &
        +x(i)*(c2(i) +x(i)*(c1(i) +x(i)*(c2(i) +x(i)*( c1(i) &
        +x(i)*(c2(i) +x(i)*(c1(i) +x(i)*(c2(i) +x(i)*( c1(i) &
        +x(i)*(c2(i) +x(i)*(c1(i) +x(i)*(c2(i) +x(i)*( c1(i) &
        ))))))))))))))))
  end subroutine poly16
end module mykernel
