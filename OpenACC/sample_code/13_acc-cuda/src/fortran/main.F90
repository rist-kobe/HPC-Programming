! Copyright 2024 Research Organization for Information Science and Technology
!======================================================================
! cuBLAS
! https://docs.nvidia.com/cuda/cublas/index.htmludd
! CUDA Fortran
! https://docs.nvidia.com/hpc-sdk/compilers/cuda-fortran-prog-guide
!======================================================================
program main
  use, intrinsic::iso_c_binding
  use cublas
  use cudafor
  use mykernel,only: dummy, initialize, mmp_simple, &
&                    mmp_simple_acc, mmp_simple_acc_gv
  use mykernel_cuda,only: mmp_cuda_wrapper
  use mytype,only: DP
  implicit none
  integer,parameter :: NITR_MAX = 10000
  integer,parameter :: iu_std = 6, iou_gen = 10
  real(kind=DP),parameter :: zero = 0.0_DP, one = 1.0_DP
  
  integer :: i, j, it, icon, ierr
  integer :: nargc, NSIZE, NITR

  character(len=32) :: cbuf

  real(kind=DP) :: gflop, gflops, tmp, diff, elp0, elp, elp1, elp2
  real(kind=DP), allocatable, dimension(:,:) :: mata
  real(kind=DP), allocatable, dimension(:,:) :: matb
  real(kind=DP), allocatable, dimension(:,:) :: matc
  real(kind=DP), allocatable, dimension(:,:) :: matcc

!======================================================================
  interface
!======================================================================
  real(kind=c_double) function get_elp_time() bind(c)
    use,intrinsic::iso_c_binding,only: c_double
  end function get_elp_time

  real(kind=c_double) function get_cpu_time() bind(c)
    use,intrinsic::iso_c_binding,only: c_double
  end function get_cpu_time
!======================================================================
  end interface
!======================================================================

  nargc = command_argument_count()
  if ( nargc .ne. 1 ) then
     write(iu_std,'(1A,/,1A)')                           &
&      "[usage] run.x (arg1) ",                          &
&      "        (arg1): matrix dimension (integer)"
     stop
  end if

  call get_command_argument(1,cbuf)
  read(cbuf,'(1I10)') NSIZE
  if ( NSIZE .le. 0 ) then
     write(iu_std, '(1A)') &
&       "Error: Maxtrix dimension must be a positive integer."
     stop
  end if

  allocate ( mata(1:NSIZE,1:NSIZE) )
  allocate ( matb(1:NSIZE,1:NSIZE) )
  allocate ( matc(1:NSIZE,1:NSIZE) )
  allocate ( matcc(1:NSIZE,1:NSIZE) )
 
  tmp = dble(NSIZE)
  gflop = 2.0_DP * tmp * tmp * tmp * 1.0E-9_DP

  NITR = NITR_MAX / NSIZE
  if ( NITR .lt. 10 ) then
     NITR = 5 
  else if ( NITR .lt. 100 ) then
     NITR = 5 * NITR
  else
     NITR = NITR_MAX
  end if 

  call initialize(matc, mata, matb, NSIZE)

  write(iu_std,'(1A24,2A10,3A10,2A23)') &
&    "kernel", "NSIZE", "NITR", "ELP_H+D", "ELP", "GFlop/s", "trace", "RMSE"
!=========================================================================
! [Simple implementation]
!=========================================================================
  elp0 = get_elp_time()

  icon = 0
  do it = 1, NITR
    call mmp_simple(matc, mata, matb, NSIZE)
    icon = icon + dummy(it, NSIZE, matc)
  end do

  elp = get_elp_time() - elp0

  gflops = (gflop*NITR) / elp

  elp2 = elp

  diff = zero

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + matc(i,i)
  end do

  write(iu_std,'(1A24,2I10,3F10.3,2E23.12)') &
&    "simple", NSIZE, NITR, elp2, elp, gflops, tmp, diff

  matcc (:, :) = matc (:, :)
  matc (:, :) = zero
#if defined(CHECK_MATMUL) 
!=========================================================================
! [Fortran Matmul]
!=========================================================================
  elp0 = get_elp_time()

  icon = 0
  do it = 1, NITR
    matc = matmul(mata, matb)
    icon = icon + dummy(it, NSIZE, matc)
  end do

  elp = get_elp_time() - elp0

  gflops = (gflop*NITR) / elp

  elp2 = elp

  diff = zero
  do i = 1, NSIZE
  do j = 1, NSIZE
     tmp = matc(i,j) - matcc(i,j)
     diff = diff + tmp*tmp
  end do
  end do
  diff = sqrt(diff)/NSIZE

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + matc(i,i)
  end do

  write(iu_std,'(1A24,2I10,3F10.3,2E23.12)') &
&    "matmul", NSIZE, NITR, elp2, elp, gflops, tmp, diff

  matc (:, :) = zero
#endif
!=========================================================================
! warm-up
!=========================================================================
  !$ACC data copyin(mata(1:NSIZE,1:NSIZE),matb(1:NSIZE,1:NSIZE)) &
  !$ACC & copy(matc(1:NSIZE,1:NSIZE))

  icon = 0
  do it = 1, 3
    call mmp_simple_acc(matc, mata, matb, NSIZE)
    icon = icon + dummy(it, NSIZE, matc)
  end do

  !$ACC end data
  matc (:, :) = zero
!=========================================================================
! [Simple implementation: OpenACC]
!=========================================================================
  elp0 = get_elp_time()

  !=======================================================================
  !$ACC data copyin(mata(1:NSIZE,1:NSIZE),matb(1:NSIZE,1:NSIZE)) &
  !$ACC & copy(matc(1:NSIZE,1:NSIZE))
  !=======================================================================
  elp1 = get_elp_time()

  icon = 0
  do it = 1, NITR
    call mmp_simple_acc(matc, mata, matb, NSIZE)
    icon = icon + dummy(it, NSIZE, matc)
  end do

  elp = get_elp_time() - elp1
  !=======================================================================
  !$ACC end data
  !=======================================================================

  elp2= get_elp_time() - elp0

  gflops = (gflop*NITR) / elp

  diff = zero
  do i = 1, NSIZE
  do j = 1, NSIZE
     tmp = matc(i,j) - matcc(i,j)
     diff = diff + tmp*tmp
  end do
  end do
  diff = sqrt(diff)/NSIZE

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + matc(i,i)
  end do

  write(iu_std,'(1A24,2I10,3F10.3,2E23.12)') &
&    "simple_acc", NSIZE, NITR, elp2, elp, gflops, tmp, diff

  matc (:, :) = zero
!=========================================================================
! [Simple implementation: OpenACC gange-vector]
!=========================================================================
  elp0 = get_elp_time()

  !=======================================================================
  !$ACC data copyin(mata(1:NSIZE,1:NSIZE),matb(1:NSIZE,1:NSIZE)) &
  !$ACC & copy(matc(1:NSIZE,1:NSIZE))
  !=======================================================================
  elp1 = get_elp_time()

  icon = 0
  do it = 1, NITR
    call mmp_simple_acc_gv(matc, mata, matb, NSIZE)
    icon = icon + dummy(it, NSIZE, matc)
  end do

  elp = get_elp_time() - elp1
  !=======================================================================
  !$ACC end data
  !=======================================================================

  elp2= get_elp_time() - elp0

  gflops = (gflop*NITR) / elp

  diff = zero
  do i = 1, NSIZE
  do j = 1, NSIZE
     tmp = matc(i,j) - matcc(i,j)
     diff = diff + tmp*tmp
  end do
  end do
  diff = sqrt(diff)/NSIZE

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + matc(i,i)
  end do

  write(iu_std,'(1A24,2I10,3F10.3,2E23.12)') &
&    "simple_acc_gv", NSIZE, NITR, elp2, elp, gflops, tmp, diff

  matc (:, :) = zero
!=========================================================================
! [my CUDA implementation]
!=========================================================================
  elp0 = get_elp_time()

  !=======================================================================
  !$ACC data copyin(mata(1:NSIZE,1:NSIZE),matb(1:NSIZE,1:NSIZE)) &
  !$ACC & copy(matc(1:NSIZE,1:NSIZE))
  !=======================================================================
  elp1 = get_elp_time()

  icon = 0
  do it = 1, NITR
    !$ACC host_data use_device(mata,matb,matc)
    call mmp_cuda_wrapper(matc, mata, matb, NSIZE)
    !$ACC end host_data
    icon = icon + dummy(it, NSIZE, matc)
  end do

  elp = get_elp_time() - elp1
  !=======================================================================
  !$ACC end data
  !=======================================================================

  elp2= get_elp_time() - elp0

  gflops = (gflop*NITR) / elp

  diff = zero 
  do i = 1, NSIZE
  do j = 1, NSIZE
     tmp = matc(i,j) - matcc(i,j)
     diff = diff + tmp*tmp
  end do
  end do
  diff = sqrt(diff)/NSIZE

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + matc(i,i)
  end do

  write(iu_std,'(1A24,2I10,3F10.3,2E23.12)') &
&    "my_cuda_impl", NSIZE, NITR, elp2, elp, gflops, tmp, diff

  matc (:, :) = zero
#if 0
! Skip... Maybe wrong.
!=========================================================================
! [DGEMM in cuBLAS]
!=========================================================================
  elp0 = get_elp_time()

  !=======================================================================
  !$ACC data copyin(mata(1:NSIZE,1:NSIZE),matb(1:NSIZE,1:NSIZE)) &
  !$ACC & copy(matc(1:NSIZE,1:NSIZE))
  !=======================================================================
  elp1 = get_elp_time()

  icon = 0
  do it = 1, NITR
    !$ACC host_data use_device(mata,matb,matc)
    call cublasDgemm('N','N', NSIZE, NSIZE, NSIZE, &
&      one, mata, NSIZE, matb, NSIZE, zero, matc, NSIZE) ! N or T?
    !$ACC end host_data

    ierr = cudaDeviceSynchronize()
    if ( ierr .ne. 0 ) then
       error stop
    end if

    icon = icon + dummy(it, NSIZE, matc)
  end do

  elp = get_elp_time() - elp1
  !=======================================================================
  !$ACC end data
  !=======================================================================

  elp2= get_elp_time() - elp0

  gflops = (gflop*NITR) / elp

  diff = zero 
  do i = 1, NSIZE
  do j = 1, NSIZE
     tmp = matc(i,j) - matcc(i,j)
     diff = diff + tmp*tmp
  end do
  end do
  diff = sqrt(diff)/NSIZE

  tmp = zero
  do i = 1, NSIZE
     tmp = tmp + matc(i,i)
  end do

  write(iu_std,'(1A24,2I10,3F10.3,2E23.12)') &
&    "cublas_dgemm", NSIZE, NITR, elp2, elp, gflops, tmp, diff

  matc (:, :) = zero
#endif

  deallocate( mata )
  deallocate( matb )
  deallocate( matc )
  deallocate( matcc )

  stop 
end program main
