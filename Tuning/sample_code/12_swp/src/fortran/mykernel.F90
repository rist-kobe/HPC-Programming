! Copyright 2024 Research Organization for Information Science and Technology
module mykernel
  use mytype,only: DP
  implicit none

contains
! ======================================================================
! Core part of Modified Gram-Schmidt (MGS) method
!
! Note: Be careful. This is NOT a full implementation of MGS.
!
! Reference:
! [1] D. S. Watkins, "The Matrix Eigenvalue Problem" 
!     (SIAM, 2007) Chap.3.
! [2] H. Sabukawa, K. Fujino, T. Nagashima, and D. Takahashi, 
!     "HPC programming" (2009) Chap.3 [In Japanese].
! [3] A. Imakura and Y. Yamamoto, "Efficient implementations of 
!     the modified Gram-Schmidt orthogonalization with a non-standard 
!     inner product", Jpn. J. Ind. Appl. Math. 36, 619-641 (2019).
! ======================================================================
  subroutine coremgs(a, q, N, M)
     real(kind=DP),intent(inout) :: a(N)
     real(kind=DP),intent(in)    :: q(N,M)
     integer,intent(in) :: N
     integer,intent(in) :: M

     integer :: i, j
     real(kind=DP) :: r

     do j = 1, M

        r = 0.0_DP
        do i = 1, N
           r = r + q(i,j)*a(i)
        end do
         
        do i = 1, N
           a(i) = a(i) - r*q(i,j)
        end do

     end do

  end subroutine coremgs
! ======================================================================
! Core part of Modified Gram-Schmidt (MGS) method:
! Application of software pipelining[2]
!
! Note: Be careful. This is NOT a full implementation of MGS.
!
! Reference:
! [1] D. S. Watkins, "The Matrix Eigenvalue Problem" 
!     (SIAM, 2007) Chap.3.
! [2] H. Sabukawa, K. Fujino, T. Nagashima, and D. Takahashi, 
!     "HPC programming" (2009) Chap.3 [In Japanese].
! [3] A. Imakura and Y. Yamamoto, "Efficient implementations of 
!     the modified Gram-Schmidt orthogonalization with a non-standard 
!     inner product", Jpn. J. Ind. Appl. Math. 36, 619-641 (2019).
! ======================================================================
  subroutine coremgs_swp(a, q, N, M)
     real(kind=DP),intent(inout) :: a(N)
     real(kind=DP),intent(in)    :: q(N,M)
     integer,intent(in) :: N
     integer,intent(in) :: M

     integer :: i, j
     real(kind=DP) :: r, s

     ! loop peeling: prologue 
     s = 0.0_DP
     do i = 1, N
        s = s + q(i,1)*a(i) ! j = 1
     end do

     ! main body  
     do j = 2, M-1

        r = 0.0_DP
        do i = 1, N
           a(i) = a(i) - s*q(i,j-1) ! j-1
           r = r + q(i,j)*a(i)      ! j
        end do

        s = r

     end do

     ! loop peeling: epilogue
     do i = 1, N
        a(i) = a(i) - s*q(i,M) ! j=M
     end do
  end subroutine coremgs_swp
! ======================================================================
! dummy function
! ======================================================================
  function dummy(A, N)
    real(kind=DP),intent(in) :: A(:)
    integer,intent(in) :: N

    integer :: dummy
    dummy = 0
  end function 
end module mykernel
