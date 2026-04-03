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
! vadd_raw: read-after-write (flow) dependence
!  vb(2) = vb(1) + va(2)
!  vb(3) = vb(2) + va(3)
!  vb(4) = vb(3) + va(4)
!=========================================================================
subroutine vadd_raw (ns, va, vb)
  use mytype,only: DP
  implicit none
  integer,intent(in)          :: ns
  real(kind=DP),intent(in)    :: va(1:ns)
  real(kind=DP),intent(inout) :: vb(1:ns)

  integer :: i
  do i = 2, ns
     vb(i) = vb(i-1) + va(i)
  end do
end subroutine vadd_raw

!=========================================================================
! vadd_war: write-after-read (anti-flow) dependence
!  vb(1) = vb(2) + va(1)
!  vb(2) = vb(3) + va(2)
!  vb(3) = vb(4) + va(3)
!=========================================================================
subroutine vadd_war (ns, va, vb)
  use mytype,only: DP
  implicit none
  integer,intent(in)          :: ns
  real(kind=DP),intent(in)    :: va(1:ns)
  real(kind=DP),intent(inout) :: vb(1:ns)

  integer :: i
  do i = 1, ns-1
     vb(i) = vb(i+1) + va(i)
  end do
end subroutine vadd_war

!=========================================================================
! vadd_asize: use of assumed-size array 
!=========================================================================
subroutine vadd_asize (ns, va, vb)
  use mytype,only: DP
  implicit none
  integer,intent(in) :: ns
  real(kind=DP)      :: va(*)
  real(kind=DP)      :: vb(*)

  integer :: i
  do i = 1, ns
     vb(i) = vb(i) + va(i)
  end do
end subroutine vadd_asize

#if defined(__USE_F08__) 
!=========================================================================
! vadd_docnrt: vadd with do concurrent in Fortran 2008
!=========================================================================
subroutine vadd_docnrt (ns, va, vb)
  use mytype,only: DP
  implicit none
  integer,intent(in)          :: ns
  real(kind=DP),intent(in)    :: va(1:ns)
  real(kind=DP),intent(inout) :: vb(1:ns)

  integer :: i
  do concurrent (i=1:ns)
     vb(i) = vb(i) + va(i)
  end do
end subroutine vadd_docnrt
#endif

#if defined(_OPENMP)
!=========================================================================
! vadd_ompsimd  
!=========================================================================
subroutine vadd_ompsimd (ns, va, vb)
  use mytype,only: DP
  implicit none
  integer,intent(in)          :: ns
  real(kind=DP),intent(in)    :: va(1:ns)
  real(kind=DP),intent(inout) :: vb(1:ns)

  integer :: i
  !$omp simd 
  do i = 1, ns
     vb(i) = vb(i) + va(i)
  end do
end subroutine vadd_ompsimd
#endif
