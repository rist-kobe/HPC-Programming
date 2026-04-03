! Copyright 2024 Research Organization for Information Science and Technology
program main
  use mytype,only: DP, NSIZE, NREP, tcmp, tcmpv
  use, intrinsic::iso_c_binding
  implicit none
  integer,parameter :: iu_std = 6
  integer :: i, ii, itmp
  integer :: ind(NSIZE)

  real(kind=DP),parameter :: zero = 0.0_DP
  real(kind=DP),parameter :: one = 1.0_DP
  real(kind=DP) :: tmp, elp0, elp
  real(kind=DP) :: va(NSIZE)
  real(kind=DP) :: vb(NSIZE)
  type(tcmp)    :: vc(NSIZE)
  type(tcmpv)   :: vd

  external dummy, vadd, vadd_lsta, vadd_aos, vadd_soa, vadd_while

!=========================================================================
  interface
!=========================================================================
  real(kind=c_double) function get_elp_time() bind(c)
    use,intrinsic::iso_c_binding,only: c_double
  end function get_elp_time
!=========================================================================
  end interface
!=========================================================================

  tmp = one / NREP
  do i = 1, NSIZE
    va(i) = tmp 
    vb(i) = zero
    vc(i)%a = tmp
    vc(i)%b = zero
    vd%a(i) = tmp
    vd%b(i) = zero
  end do
  
  do i = 1, NSIZE
    ii = i + 46
    if ( ii > NSIZE ) ii = ii - NSIZE
    ind(i) = ii
  end do
!=========================================================================
  write(iu_std,'(1A16,1A14,1A23)') "Kernel","elapsed(sec)", "check"
!=========================================================================
  elp0 = get_elp_time()

  do i = 1, NREP
    call vadd (NSIZE,va,vb) 
    call dummy (NSIZE,va,vb,itmp)
    itmp = itmp + i
  end do

  elp = get_elp_time() - elp0

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + vb(i) 
  end do
  
  write(iu_std,'(1A16,1F14.4,1E23.12)') "vadd", elp, tmp

  do i = 1, NSIZE
    vb(i) = zero 
  end do
!=========================================================================
  elp0 = get_elp_time()

  do i = 1, NREP
    call vadd_lsta (NSIZE,ind,va,vb) 
    call dummy (NSIZE,va,vb,itmp)
    itmp = itmp + i
  end do

  elp = get_elp_time() - elp0

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + vb(i) 
  end do
  
  write(iu_std,'(1A16,1F14.4,1E23.12)') "vadd_lsta", elp, tmp

  do i = 1, NSIZE
    vb(i) = zero 
  end do
!=========================================================================
  elp0 = get_elp_time()

  do i = 1, NREP
    call vadd_aos (NSIZE,vc) 
    call dummy (NSIZE,va,vb,itmp)
    itmp = itmp + i
  end do

  elp = get_elp_time() - elp0

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + vc(i)%b
  end do
  
  write(iu_std,'(1A16,1F14.4,1E23.12)') "vadd_aos", elp, tmp

  do i = 1, NSIZE
    vc(i)%b = zero 
  end do
!=========================================================================
  elp0 = get_elp_time()

  do i = 1, NREP
    call vadd_soa (NSIZE,vd) 
    call dummy (NSIZE,va,vb,itmp)
    itmp = itmp + i
  end do

  elp = get_elp_time() - elp0

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + vd%b(i)
  end do
  
  write(iu_std,'(1A16,1F14.4,1E23.12)') "vadd_soa", elp, tmp

  do i = 1, NSIZE
    vd%b(i) = zero 
  end do
!=========================================================================
  elp0 = get_elp_time()

  do i = 1, NREP
    call vadd_while (NSIZE,va,vb) 
    call dummy (NSIZE,va,vb,itmp)
    itmp = itmp + i
  end do

  elp = get_elp_time() - elp0

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + vb(i) 
  end do
  
  write(iu_std,'(1A16,1F14.4,1E23.12)') "vadd_while", elp, tmp

  do i = 1, NSIZE
    vb(i) = zero 
  end do
!=========================================================================
  stop 
end program main
