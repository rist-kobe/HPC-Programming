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

module force_kernels
  use mytype,only: DP, NSIZE
  implicit none
contains

  ! Pair-force kernel: serial reference
  subroutine force_serial(x, f)
    real(kind=DP),intent(in) :: x(NSIZE)
    real(kind=DP),intent(inout) :: f(NSIZE)
    integer :: i, j
    real(kind=DP) :: fij

    do i=1,NSIZE-1
    do j=i+1,NSIZE
       fij = 1.0_DP/abs(x(j)-x(i))
       f(i) = f(i) +  fij
       f(j) = f(j) -  fij
    end do
    end do
  end subroutine force_serial

  ! Pair-force kernel: static scheduling
  subroutine force_static(x, f)
    real(kind=DP),intent(in) :: x(NSIZE)
    real(kind=DP),intent(inout) :: f(NSIZE)
    integer :: i, j
    real(kind=DP) :: fij

    !$omp parallel do schedule(static) private(fij) reduction(+:f)
    do i=1,NSIZE-1
    do j=i+1,NSIZE
       fij = 1.0_DP/abs(x(j)-x(i))
       f(i) = f(i) +  fij
       f(j) = f(j) -  fij
    end do
    end do
    !$omp end parallel do
  end subroutine force_static

  ! Pair-force kernel: static scheduling with chunk size 1
  subroutine force_static1(x, f)
    real(kind=DP),intent(in) :: x(NSIZE)
    real(kind=DP),intent(inout) :: f(NSIZE)
    integer :: i, j
    real(kind=DP) :: fij

    !$omp parallel do schedule(static,1) private(fij) reduction(+:f)
    do i=1,NSIZE-1
    do j=i+1,NSIZE
       fij = 1.0_DP/abs(x(j)-x(i))
       f(i) = f(i) +  fij
       f(j) = f(j) -  fij
    end do
    end do
    !$omp end parallel do
  end subroutine force_static1

  ! Pair-force kernel: dynamical scheduling with chunk size 1
  subroutine force_dynamic1(x, f)
    real(kind=DP),intent(in) :: x(NSIZE)
    real(kind=DP),intent(inout) :: f(NSIZE)
    integer :: i, j
    real(kind=DP) :: fij

    !$omp parallel do schedule(dynamic,1) private(fij) reduction(+:f)
    do i=1,NSIZE-1
    do j=i+1,NSIZE
       fij = 1.0_DP/abs(x(j)-x(i))
       f(i) = f(i) +  fij
       f(j) = f(j) -  fij
    end do
    end do
    !$omp end parallel do
  end subroutine force_dynamic1

  ! Pair-force kernel: guided scheduling
  subroutine force_guided(x, f)
    real(kind=DP),intent(in) :: x(NSIZE)
    real(kind=DP),intent(inout) :: f(NSIZE)
    integer :: i, j
    real(kind=DP) :: fij

    !$omp parallel do schedule(guided) private(fij) reduction(+:f)
    do i=1,NSIZE-1
    do j=i+1,NSIZE
       fij = 1.0_DP/abs(x(j)-x(i))
       f(i) = f(i) +  fij
       f(j) = f(j) -  fij
    end do
    end do
    !$omp end parallel do
  end subroutine force_guided

end module force_kernels

program main
  use mytype,only: DP, NSIZE
  use force_kernels
  use omp_lib
  implicit none
  integer :: i, nseed
  integer,allocatable :: seed(:)
  real(kind=DP) :: t, ts
  real(kind=DP) :: f(NSIZE),fref(NSIZE),x(NSIZE)

  ! Fixed seed for reproducible input data
  call random_seed(size=nseed)
  allocate(seed(nseed))
  seed(:) = 12345
  call random_seed(put=seed)
  deallocate(seed)

  call random_number(x)

  write(6,'("Number of threads                       =",1I9)') &
  &     omp_get_max_threads()

  ! Warm-up: create the thread team before the timed runs
  !$omp parallel
  ! Do nothing
  !$omp end parallel

  ! Serial run
  fref(:)=0.0_DP
  ts = omp_get_wtime()
  call force_serial(x, fref)
  ts = omp_get_wtime() - ts
  write(6,'("[Serial] Elapsed time (sec)             =",1F9.3)') ts

  ! Parallel run: static scheduling
  f(:)=0.0_DP
  t = omp_get_wtime()
  call force_static(x, f)
  t = omp_get_wtime() - t
  write(6,'("[OMP static] Elapsed time (sec)         =",1F9.3,&
  &     ", Speedup:",1F7.1,", Max diff:",1ES10.2)') &
  &     t, ts/t, maxval(abs(f-fref))

  ! Parallel run: static scheduling with chunk size 1
  f(:)=0.0_DP
  t = omp_get_wtime()
  call force_static1(x, f)
  t = omp_get_wtime() - t
  write(6,'("[OMP static chunk 1] Elapsed time (sec) =",1F9.3,&
  &     ", Speedup:",1F7.1,", Max diff:",1ES10.2)') &
  &     t, ts/t, maxval(abs(f-fref))

  ! Parallel run: dynamical scheduling with chunk size 1
  f(:)=0.0_DP
  t = omp_get_wtime()
  call force_dynamic1(x, f)
  t = omp_get_wtime() - t
  write(6,'("[OMP dynamic chunk 1] Elapsed time (sec)=",1F9.3,&
  &     ", Speedup:",1F7.1,", Max diff:",1ES10.2)') &
  &     t, ts/t, maxval(abs(f-fref))

  ! Parallel run: guided scheduling
  f(:)=0.0_DP
  t = omp_get_wtime()
  call force_guided(x, f)
  t = omp_get_wtime() - t
  write(6,'("[OMP guided] Elapsed time (sec)         =",1F9.3,&
  &     ", Speedup:",1F7.1,", Max diff:",1ES10.2)') &
  &     t, ts/t, maxval(abs(f-fref))

end program main
