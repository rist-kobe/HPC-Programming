! Copyright 2024 Research Organization for Information Science and Technology
!-----------------------------------------------------
! Counter of a negative value
! The original idea come from Fig 10.7 in Sec.10.2
! in T. G. Mattson, Y. (Helen) He, and A. E. Kniges,
! The OpenMP Common core: Making OpenMP Simple Again
! (MIT,2019)
! http://www.ompcore.com
!-----------------------------------------------------
module myvar
  integer icount
  !$omp threadprivate(icount)

  contains
  subroutine inc ()
    icount = icount + 1
  end subroutine inc

  integer function func ( x ) result(ret)
    integer,intent(in) :: x
    ret = x
  end function func
end module myvar

program main
  use omp_lib
  use myvar,only: icount, inc, func
  implicit none
  integer :: i
  integer :: tid
  integer :: nargc
  integer :: nsize
  integer :: v
  integer,allocatable,dimension(:) :: a
  character(len=16) :: cbuf

  nargc = command_argument_count ()

  if ( nargc .ne. 1 ) then
    write (6,'("[usage] run.x (arg1)",/,&
&          "   arg1: array size (integer)")')
    stop 1
  end if

  call get_command_argument (1, cbuf)
  read (cbuf,*) nsize
  if ( nsize <= 0 ) then
    write (6,'("Error: array size must be a positive integer.")')
    stop 1
  end if

  allocate ( a(1:nsize) )

  do i = 1, nsize
    a(i) = i-1 - nsize/2
  end do

  ! Disable dynamic thread adjustment: the threadprivate values
  ! persist between the two parallel regions only when dynamic
  ! adjustment is off and the number of threads is unchanged.
  call omp_set_dynamic ( .false. )

  icount = 0
  ! 1st round
  !$omp parallel private(v,tid) copyin(icount)
  tid = omp_get_thread_num () 
  !$omp do schedule(static) 
  do i = 1, nsize
    v = func ( a(i) ) 
    if ( v .lt. 0 ) call inc () 
  end do
  !$omp end do

  !$omp critical
  write (6,'("Round 1: thread ID=",1I0,",  # of negative v: ",1I4)') &
&        tid, icount
  !$omp end critical
  !$omp end parallel

  ! dump icount on thread ID=0
  write (6,'("b/w Rounds 1 and 2: primary thread, # of negative v: ",1I4)') icount

  ! 2nd round: copyin is deliberately omitted, so each thread's
  ! icount retains its value from Round 1 and keeps accumulating.
  !$omp parallel private(v,tid)
  tid = omp_get_thread_num () 
  !$omp do schedule(static) 
  do i = 1, nsize
    v = func ( a(i) ) 
    if ( v .lt. 0 ) call inc () 
  end do
  !$omp end do

  !$omp critical
  write (6,'("Round 2: thread ID=",1I0,",  # of negative v: ",1I4)') &
&        tid, icount
  !$omp end critical
  !$omp end parallel

  ! Reference: number of negatives per round for a self-check
  write (6,'("Expected # of negative v per round: ",1I4)') nsize/2

end program main
