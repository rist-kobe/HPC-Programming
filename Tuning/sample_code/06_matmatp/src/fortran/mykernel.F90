! Copyright 2024 Research Organization for Information Science and Technology
module mykernel
  use mytype, only: DP
  implicit none

contains
!=========================================================================
!  Simple implementation
!  * Memory access pattern 
!        stride  : non-contiguous access with stride > 1
!        cont    : contiguous access (stride 1 access)
!        register: on register 
! ====================================================
! loop depth |     MC         MA         MB
!  j         | stride      register    stride 
!  k         | register    stride      cont  
!  i         | cont        cont        register 
!=========================================================================
subroutine mmp_simple (MC, MA, MB, ns)
  integer,intent(in) :: ns 
  real(kind=DP),intent(in) :: MA(ns,ns), MB(ns,ns)
  real(kind=DP),intent(out) :: MC(ns,ns)

  integer :: i, j, k

  MC(:,:) = 0.0_DP

  do j = 1, ns
  do k = 1, ns
  do i = 1, ns
     MC(i,j) = MC(i,j) + MA(i,k)*MB(k,j) 
  end do
  end do
  end do
end subroutine mmp_simple

!=========================================================================
!  Simple loop blocking
!  * Memory access pattern 
!        stride  : non-contiguous access with stride > 1
!        cont    : contiguous access (stride 1 access)
!        register: on register 
! ====================================================
! loop depth |     MC         MA         MB
!  j         | stride      register    stride 
!  k         | register    stride      cont  
!  i         | cont        cont        register 
!=========================================================================
subroutine mmp_simple_blk (MC, MA, MB, ns, nbk1, nbk2)
  integer,intent(in) :: ns, nbk1, nbk2
  real(kind=DP),intent(in) :: MA(ns,ns), MB(ns,ns)
  real(kind=DP),intent(out) :: MC(ns,ns)

  integer :: i, j, k, ii, kk

  MC(:,:) = 0.0_DP
  
  !   (cashe size per core/8bytes) > nbk1*nbk2 + nbk1 + nbk2
  do kk = 1, ns, nbk2
  do ii = 1, ns, nbk1

  do j = 1, ns
  do k = kk, min(ns, kk+nbk2-1)
  do i = ii, min(ns, ii+nbk1-1)
        MC(i,j) = MC(i,j) + MA(i,k)*MB(k,j) 
  end do
  end do
  end do

  end do
  end do
end subroutine mmp_simple_blk

!=========================================================================
!  Exchanging loop, Transpose, and Blocking (two inner loops) 
!  * Memory access pattern 
!        stride  : non-contiguous access with stride > 1
!        cont    : contiguous access (stride 1 access)
!        register: on register 
! ====================================================
! loop depth |     MC        WMA         MB
!  j         | stride      register    stride 
!  i         | cont        stride      register 
!  k         | register    cont        cont  
!=========================================================================
subroutine mmp_lex_tp_blk (MC, MA, MB, ns, nbk1, nbk2, nbkr, nbkc)
  integer,intent(in) :: ns , nbk1, nbk2
  integer,intent(in) :: nbkr, nbkc
  real(kind=DP),intent(in) :: MA(ns,ns), MB(ns,ns)
  real(kind=DP),intent(out) :: MC(ns,ns)

  integer :: i, j, k, ii, kk
  real(kind=DP) :: WMA(ns,ns)

#if ! defined(NOT_USE_TRANS_BLK)
  call trans_blk(WMA,MA,ns,ns,nbkr,nbkc)
#else
  call trans(WMA,MA,ns,ns)
#endif
  
  MC(:,:) = 0.0_DP

  !   (cashe size per core/8bytes) > nbk1*nbk2 + nbk1 
  do ii = 1, ns, nbk2
  do kk = 1, ns, nbk1
  
     do j = 1, ns
     do i = ii, min(ns,ii+nbk2-1)
        do k = kk, min(ns,kk+nbk1-1)
           MC(i,j) = MC(i,j) + WMA(k,i)*MB(k,j) 
        end do
     end do
     end do
  
  end do
  end do
end subroutine mmp_lex_tp_blk

!=========================================================================
!  Transpose routine
!=========================================================================
subroutine trans( TM, M, nraw, ncol )
  integer,intent(in) :: nraw, ncol
  real(kind=DP),intent(in) :: M(nraw,ncol)
  real(kind=DP),intent(out) :: TM(ncol,nraw)

  integer :: i, j

  do j = 1, nraw 
  do i = 1, ncol 
     TM(i,j) = M(j,i)
  end do
  end do
end subroutine trans

!=========================================================================
!  Transpose routine with blocking
!=========================================================================
subroutine trans_blk( TM, M, nraw, ncol,nbkr,nbkc)
  integer,intent(in) :: nraw, ncol
  integer,intent(in) :: nbkr, nbkc
  real(kind=DP),intent(in) :: M(nraw,ncol)
  real(kind=DP),intent(out) :: TM(ncol,nraw)

  integer :: i, j, ii, jj

  do jj = 1, nraw, nbkr
  do ii = 1, ncol, nbkc 
     do j = jj, min(nraw,jj+nbkr-1)
     do i = ii, min(ncol,ii+nbkc-1)
        TM(i,j) = M(j,i)
     end do
     end do
  end do
  end do
end subroutine trans_blk

!=========================================================================
!  Dummy routine
!=========================================================================
subroutine dummy (M, nraw, ncol, s)
  integer,intent(in) :: nraw, ncol
  real(kind=DP),intent(in) :: M(nraw,ncol)
  integer,intent(out)         :: s
  s = 0
end subroutine dummy

end module mykernel
