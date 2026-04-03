! Copyright 2024 Research Organization for Information Science and Technology
!-----------------------------------------------------
! Reduce load imbalance 
! Author:  Yukihiro Ota (yota@rist.or.jp)
! Description:  
!          This is a miniature of calculating newtonian 
!          pair force in MD simulation. The original 
!          code was shown by D. Yamaki and Y. Aoyama at RIST.
!-----------------------------------------------------
module mytype
  integer,parameter :: SP = kind(1.0)
  integer,parameter :: DP = selected_real_kind(2*precision(1.0_SP))
  integer,parameter :: NSIZE = 80000
end module mytype

program main
  use mytype,only: DP, NSIZE
  use omp_lib
  implicit none
  integer :: i, j
  real(kind=DP) :: fij, t, ts
  real(kind=DP) :: f(NSIZE),x(NSIZE)

  call random_number(x)

  ! Serial run
  f(:)=0.0_DP
  ts = omp_get_wtime()
  do i=1,NSIZE-1
  do j=i+1,NSIZE
     fij = 1.0_DP/abs(x(j)-x(i))
     f(i) = f(i) +  fij
     f(j) = f(j) -  fij
  end do
  end do
  ts = omp_get_wtime() - ts
  write(6,'("[Serial] Elapsed time (sec)             =",1F9.3)') ts

  ! Parallel run: static scheduling
  f(:)=0.0_DP
  t = omp_get_wtime()
  !$omp parallel do schedule(static) private(fij) reduction(+:f)
  do i=1,NSIZE-1
  do j=i+1,NSIZE
     fij = 1.0_DP/abs(x(j)-x(i))
     f(i) = f(i) +  fij
     f(j) = f(j) -  fij
  end do
  end do
  !$omp end parallel do
  t = omp_get_wtime() - t
  write(6,'("[OMP static] Elapsed time (sec)         =",1F9.3,&
  &     ", Speedup:",1F7.1)') t, ts/t

  ! Parallel run: static scheduling with chunk size 1
  f(:)=0.0_DP
  t = omp_get_wtime()
  !$omp parallel do schedule(static,1) private(fij) reduction(+:f)
  do i=1,NSIZE-1
  do j=i+1,NSIZE
     fij = 1.0_DP/abs(x(j)-x(i))
     f(i) = f(i) +  fij
     f(j) = f(j) -  fij
  end do
  end do
  !$omp end parallel do
  t = omp_get_wtime() - t
  write(6,'("[OMP static chunk 1] Elapsed time (sec) =",1F9.3,&
  &     ", Speedup:",1F7.1)') t, ts/t

  ! Parallel run: dynamical scheduling with chunk size 1
  f(:)=0.0_DP
  t = omp_get_wtime()
  !$omp parallel do schedule(dynamic,1) private(fij) reduction(+:f)
  do i=1,NSIZE-1
  do j=i+1,NSIZE
     fij = 1.0_DP/abs(x(j)-x(i))
     f(i) = f(i) +  fij
     f(j) = f(j) -  fij
  end do
  end do
  !$omp end parallel do
  t = omp_get_wtime() - t
  write(6,'("[OMP dynamic chunk 1] Elapsed time (sec)=",1F9.3,&
  &     ", Speedup:",1F7.1)') t, ts/t

  stop
end program main
