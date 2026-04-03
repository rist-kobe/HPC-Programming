! Copyright 2025 Research Organization for Information Science and Technology
program main
  use mytype,only: SP, DP, NSIZE, NREP
#if defined(_OPENMP)
  use omp_lib
#endif
#if defined(_OPENACC)
  use openacc
#endif
  implicit none
  
  integer :: i
  integer :: s

  real(kind=DP) :: elp
  real(kind=DP) :: elp0
  real(kind=SP) :: tmp
  real(kind=SP),allocatable,dimension(:) :: x
  real(kind=SP),allocatable,dimension(:) :: y

  external saxpy_0, saxpy_omp_0, saxpy_1, dummy

  allocate( x(1:NSIZE) )
  allocate( y(1:NSIZE) )

  BLOCK
  integer :: nt, ng
#if defined(_OPENACC)
  integer(kind=acc_device_kind) :: dev_type
#endif
#if defined(_OPENMP)
  write(6,'("openmp_version=",1I10)') openmp_version   
  nt = omp_get_max_threads() 
  write(6,'("Number of OpenMP host threads     : ",1I3)') nt
#endif
#if defined(_OPENACC)
  write(6,'("openacc_version=",1I10)') openacc_version   
  dev_type = acc_get_device_type()
  ng = acc_get_num_devices(dev_type)
  if ( ng .eq. 0 ) then
     write(6,'("There is no available device.")')
     stop
  end if
  write(6,'("Number of OpenACC capable devices : ",1I3)') ng
#endif
  END BLOCK

  ! saxpy_0
  !$OMP PARALLEL DO SCHEDULE(STATIC)
  do i = 1, NSIZE
    x(i) = 1.0_SP
    y(i) = 2.0_SP
  end do
  !$OMP END PARALLEL DO

  call saxpy_0(NSIZE,3.0_SP,x,y) ! warm-up

  elp0 = omp_get_wtime()

  do i = 1, NREP
    call saxpy_0(NSIZE,3.0_SP,x,y)
    call dummy(NSIZE,x,s)
    s = s + i
  end do

  elp = omp_get_wtime() - elp0

  tmp = 0.0
  do i = 1, NSIZE
    tmp = tmp + y(i)
  end do

  write(6,'("[SAXPY_0] elp_sec:",1F10.3,1X,"check:",1E15.8)') elp, tmp

  ! saxpy_1
  !$OMP PARALLEL DO SCHEDULE(STATIC)
  do i = 1, NSIZE
    x(i) = 1.0_SP
    y(i) = 2.0_SP
  end do
  !$OMP END PARALLEL DO

  call saxpy_1(NSIZE,3.0_SP,x,y) ! warm-up

  elp0 = omp_get_wtime()

  do i = 1, NREP
    call saxpy_1(NSIZE,3.0_SP,x,y)
    call dummy(NSIZE,x,s)
    s = s + i
  end do

  elp = omp_get_wtime() - elp0

  tmp = 0.0
  do i = 1, NSIZE
    tmp = tmp + y(i)
  end do

  write(6,'("[SAXPY_1] elp_sec:",1F10.3,1X,"check:",1E15.8)') elp, tmp

  ! saxpy_omp_0
  !$OMP PARALLEL DO SCHEDULE(STATIC)
  do i = 1, NSIZE
    x(i) = 1.0_SP
    y(i) = 2.0_SP
  end do
  !$OMP END PARALLEL DO

  call saxpy_omp_0(NSIZE,3.0_SP,x,y) ! warm-up

  elp0 = omp_get_wtime()

  do i = 1, NREP
    call saxpy_omp_0(NSIZE,3.0_SP,x,y)
    call dummy(NSIZE,x,s)
    s = s + i
  end do

  elp = omp_get_wtime() - elp0

  tmp = 0.0
  do i = 1, NSIZE
    tmp = tmp + y(i)
  end do

  write(6,'("[SAXPY_OMP_0] elp_sec:",1F10.3,1X,"check:",1E15.8)') elp, tmp
  deallocate ( x )
  deallocate ( y )
  stop
end program main
