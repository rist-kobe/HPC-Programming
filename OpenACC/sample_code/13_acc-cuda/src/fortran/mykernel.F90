! Copyright 2024 Research Organization for Information Science and Technology
module mykernel
   use mytype, only: DP
   implicit none
   real(kind=DP),parameter :: zero = 0.0_DP, one = 1.0_DP
   
contains
!=========================================================================
!  Initialize
!=========================================================================
   subroutine initialize (MC, MA, MB, ns)
     integer,intent(in) :: ns 
     real(kind=DP),intent(out) :: MC(ns,ns), MA(ns,ns), MB(ns,ns)
   
     integer :: i, j, nseed
     integer,allocatable,dimension(:) :: seed
     real(kind=DP) :: rnd(2)
     real(kind=DP) :: tmp
   
     call random_seed(size=nseed)
   
     allocate ( seed(1:nseed) )
   
     do i = 1, nseed
        !call system_clock(count=seed(i))
        seed(i) = 20409102 + i - 1
     end do
     call random_seed(put=seed)
   
     tmp = one / ns
     do j = 1, ns
     do i = 1, ns
        call random_number(rnd)
        MA(i,j) = rnd(1) * tmp 
        MB(i,j) = rnd(2) * tmp
        MC(i,j) = zero
     end do
     end do
   
     deallocate( seed )
   end subroutine initialize
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
!  Simple implementation with OpenACC
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
   subroutine mmp_simple_acc (MC, MA, MB, ns)
     integer,intent(in) :: ns 
     real(kind=DP),intent(in) :: MA(ns,ns), MB(ns,ns)
     real(kind=DP),intent(out) :: MC(ns,ns)
   
     integer :: i, j, k
     real(kind=DP) :: tmp

     !$ACC parallel present(MA(1:ns,1:ns),MB(1:ns,1:ns),MC(1:ns,1:ns)) &
     !$ACC & private(i,j,k,tmp)

     !$ACC loop independent collapse(2)
     do j = 1, ns
     do i = 1, ns
        MC(i,j) =0.0_DP
     end do
     end do
     !$ACC end loop
   
     !$ACC loop independent collapse(2)
     do j = 1, ns
     do i = 1, ns
       tmp = 0.0_DP
       !$ACC loop seq
       do k = 1, ns
          tmp = tmp + MA(i,k)*MB(k,j) 
       end do
       !$ACC end loop
       MC(i,j) = MC(i,j) + tmp
     end do
     end do
     !$ACC end loop

     !$ACC end parallel
   end subroutine mmp_simple_acc
!=========================================================================
!  Simple implementation with OpenACC: gang-vector
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
   subroutine mmp_simple_acc_gv (MC, MA, MB, ns)
     integer,intent(in) :: ns 
     real(kind=DP),intent(in) :: MA(ns,ns), MB(ns,ns)
     real(kind=DP),intent(out) :: MC(ns,ns)
   
     integer :: i, j, k
     real(kind=DP) :: tmp

     !$ACC parallel present(MA(1:ns,1:ns),MB(1:ns,1:ns),MC(1:ns,1:ns)) &
     !$ACC & private(i,j,k,tmp)

     !$ACC loop independent collapse(2)
     do j = 1, ns
     do i = 1, ns
        MC(i,j) =0.0_DP
     end do
     end do
     !$ACC end loop
   
     !$ACC loop gang independent
     do j = 1, ns
     !$ACC loop vector independent
     do i = 1, ns
       tmp = 0.0_DP
       !$ACC loop seq
       do k = 1, ns
          tmp = tmp + MA(i,k)*MB(k,j) 
       end do
       !$ACC end loop
       MC(i,j) = MC(i,j) + tmp
     end do
     !$ACC end loop
     end do
     !$ACC end loop

     !$ACC end parallel
   end subroutine mmp_simple_acc_gv
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
! dummy 
!=========================================================================
   function dummy(it, ns, mat) result(icon)
      integer :: it
      integer :: ns
      real(kind=DP) :: mat(1:ns,1:ns)
      integer :: icon
      icon = 0
   end function dummy
end module mykernel
