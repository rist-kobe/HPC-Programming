! Copyright 2024 Research Organization for Information Science and Technology
program main
  use mykernel,only: saxpy_0, saxpy_1, saxpy_2, dummy
  use mytype,only: SP, DP, NSIZE, NREP
  use timer,only: get_elp_time
  implicit none

  abstract interface
    subroutine saxpy_kernel (ns, a, x, y)
      import :: SP
      integer,intent(in) :: ns
      real(kind=SP),contiguous,intent(inout)  :: y(:)
      real(kind=SP),intent(in)                :: a
      real(kind=SP),contiguous,intent(in)     :: x(:)
    end subroutine saxpy_kernel
  end interface

  real(kind=SP),allocatable,dimension(:) :: x
  real(kind=SP),allocatable,dimension(:) :: y

  allocate( x(1:NSIZE) )
  allocate( y(1:NSIZE) )

  call run_benchmark('SAXPY_0', saxpy_0, .false.)
  call run_benchmark('SAXPY_1', saxpy_1, .false.)
  call run_benchmark('SAXPY_2', saxpy_2, .true.)

  deallocate ( x )
  deallocate ( y )

contains

  ! Expected checksum for one pass: NSIZE * a * 0.0001 = 0.3072
  subroutine run_benchmark (label, kernel, use_data_region)
    character(len=*),intent(in) :: label
    procedure(saxpy_kernel) :: kernel
    logical,intent(in) :: use_data_region

    integer :: i, irep
    integer :: s
    integer :: ielp0
    real(kind=DP) :: elp
    real(kind=SP) :: tmp

    do i = 1, NSIZE
      x(i) = 0.0001_SP
    end do
    y(:) = 0.0_SP

    call kernel(NSIZE,3.0_SP,x,y) ! warm-up

    call get_elp_time(ielp0)

    s = 0

#if _OPENACC
    !$ACC data copyin(x(1:NSIZE)) if(use_data_region)
#endif
#if _OPENMP
    !$OMP target data map(to:x(1:NSIZE)) if(use_data_region)
#endif
    do irep = 1, NREP
      y(:) = 0.0_SP
      call kernel(NSIZE,3.0_SP,x,y)
      s = s +  dummy(NSIZE,x)
    end do
#if _OPENMP
    !$OMP end target data
#endif
#if _OPENACC
    !$ACC end data
#endif

    call get_elp_time(ielp0, elp)

    tmp = 0.0
    do i = 1, NSIZE
      tmp = tmp + y(i)
    end do

    write(6,'("[",A,"] elapsed_sec: ",F10.3," check: ",E15.8," s: ",I0)') &
      label, elp, tmp, s
  end subroutine run_benchmark

end program main
