! Copyright 2024 Research Organization for Information Science and Technology
!=========================================================================
! vadd 
!=========================================================================
subroutine vadd (ns, va, vb)
  use mytype,only: DP
  implicit none
  integer,intent(in)          :: ns
  real(kind=DP),intent(in)    :: va(1:ns)
  real(kind=DP),intent(inout) :: vb(1:ns)

  integer :: i
  do i = 1, ns
     vb(i) = vb(i) + va(i)
  end do
end subroutine vadd 

!=========================================================================
! vadd_lsta: vadd with indirect access via an index list 
!=========================================================================
subroutine vadd_lsta (ns, ind, va, vb)
  use mytype,only: DP
  implicit none
  integer,intent(in)          :: ns
  integer,intent(in)          :: ind(1:ns)
  real(kind=DP),intent(in)    :: va(1:ns)
  real(kind=DP),intent(inout) :: vb(1:ns)

  integer :: i, ii
#if defined(__USE_DIRECTIVES__)
#if ( __GNUC__ > 5 ) && defined(_OPENMP)
  !$omp simd 
#endif
#endif
  do i = 1, ns
     ii = ind(i) 
     vb(i) = vb(ii) + va(i)
  end do
end subroutine vadd_lsta

!=========================================================================
! vadd_aos: vadd with Structure of Array
!=========================================================================
subroutine vadd_aos (ns, v)
  use mytype,only: DP, tcmp 
  implicit none
  integer,intent(in)          :: ns
  type(tcmp),intent(inout)   :: v(1:ns)

  integer :: i
  do i = 1, ns
     v(i)%b = v(i)%b + v(i)%a
  end do
end subroutine vadd_aos 

!=========================================================================
! vadd_soa: vadd with Array of Structure
!=========================================================================
subroutine vadd_soa (ns, v)
  use mytype,only: DP, tcmpv
  implicit none
  integer,intent(in)          :: ns
  type(tcmpv),intent(inout)   :: v

  integer :: i
  do i = 1, ns
     v%b(i) = v%b(i) + v%a(i)
  end do
end subroutine vadd_soa

!=========================================================================
! vadd_while: vadd with do while
!=========================================================================
subroutine vadd_while (ns, va, vb)
  use mytype,only: DP
  implicit none
  integer,intent(in)          :: ns
  real(kind=DP),intent(in)    :: va(1:ns)
  real(kind=DP),intent(inout) :: vb(1:ns)

  integer :: i
  i = 1
  do while ( i .le. ns )
     vb(i) = vb(i) + va(i)
     i = i + 1
  end do 
end subroutine vadd_while 
