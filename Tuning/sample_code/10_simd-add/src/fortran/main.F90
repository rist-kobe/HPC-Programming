! Copyright 2024 Research Organization for Information Science and Technology
program main
  use mytype,only: DP
  use, intrinsic::iso_c_binding
  implicit none
#define NSIZE 32000 
#define NREP 100000
  integer,parameter :: iu_std = 6
  integer :: i, itmp

  real(kind=DP),parameter :: zero = 0.0_DP
  real(kind=DP),parameter :: one = 1.0_DP
  real(kind=DP) :: tmp, elp0, elp
  real(kind=DP) :: va(NSIZE)
  real(kind=DP) :: vb(NSIZE)

  external dummy, vadd, vadd_raw, vadd_war, vadd_asize
#if defined(__USE_F08__) 
  external vadd_docnrt
#endif
#if defined(_OPENMP)
  external vadd_ompsimd
#endif

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
    call vadd_raw (NSIZE,va,vb) 
    call dummy (NSIZE,va,vb,itmp)
    itmp = itmp + i
  end do

  elp = get_elp_time() - elp0

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + vb(i) 
  end do
  
  write(iu_std,'(1A16,1F14.4,1E23.12)') "vadd_raw", elp, tmp

  do i = 1, NSIZE
    vb(i) = zero 
  end do
!=========================================================================
  elp0 = get_elp_time()

  do i = 1, NREP
    call vadd_war (NSIZE,va,vb) 
    call dummy (NSIZE,va,vb,itmp)
    itmp = itmp + i
  end do

  elp = get_elp_time() - elp0

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + vb(i) 
  end do
  
  write(iu_std,'(1A16,1F14.4,1E23.12)') "vadd_war", elp, tmp

  do i = 1, NSIZE
    vb(i) = zero 
  end do
!=========================================================================
  elp0 = get_elp_time()

  do i = 1, NREP
    call vadd_asize (NSIZE,va,vb) 
    call dummy (NSIZE,va,vb,itmp)
    itmp = itmp + i
  end do

  elp = get_elp_time() - elp0

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + vb(i) 
  end do
  
  write(iu_std,'(1A16,1F14.4,1E23.12)') "vadd_asize", elp, tmp

  do i = 1, NSIZE
    vb(i) = zero 
  end do
!=========================================================================
#if defined(__USE_F08__) 
  elp0 = get_elp_time()

  do i = 1, NREP
    call vadd_docnrt (NSIZE,va,vb) 
    call dummy (NSIZE,va,vb,itmp)
    itmp = itmp + i
  end do

  elp = get_elp_time() - elp0

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + vb(i) 
  end do
  
  write(iu_std,'(1A16,1F14.4,1E23.12)') "vadd_docnrt", elp, tmp

  do i = 1, NSIZE
    vb(i) = zero 
  end do
#endif
!=========================================================================
#if defined(_OPENMP)
  elp0 = get_elp_time()

  do i = 1, NREP
    call vadd_ompsimd (NSIZE,va,vb) 
    call dummy (NSIZE,va,vb,itmp)
    itmp = itmp + i
  end do

  elp = get_elp_time() - elp0

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + vb(i) 
  end do
  
  write(iu_std,'(1A16,1F14.4,1E23.12)') "vadd_ompsimd", elp, tmp

  do i = 1, NSIZE
    vb(i) = zero 
  end do
#endif
!=========================================================================
  stop 
end program main
