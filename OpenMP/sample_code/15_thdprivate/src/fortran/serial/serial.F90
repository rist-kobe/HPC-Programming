! Copyright 2024 Research Organization for Information Science and Technology
module myvar
  integer icount

  contains
  subroutine inc ()
    icount = icount + 1
  end subroutine inc
end module myvar

integer function func ( x ) result(ret)
  integer,intent(in) :: x
  ret = x
  return
end function func

program main
  use myvar,only: icount, inc
  implicit none
  integer :: i
  integer :: nargc
  integer :: nsize
  integer :: v
  integer :: func
  integer,allocatable,dimension(:) :: a
  character(len=16) :: cbuf

  nargc = command_argument_count ()

  if ( nargc .ne. 1 ) then
    write (6,'("[usage] run.x (arg1) \n",&
&          "   arg1: array size (integer)")')
    stop
  end if

  call get_command_argument (1, cbuf)
  read (cbuf,'(1I10)') nsize
  if ( nsize < 0 ) then
    stop
  end if

  allocate ( a(1:nsize) )

  do i = 1, nsize
    a(i) = i-1 - nsize/2
  end do

  icount = 0

  do i = 1, nsize
    v = func ( a(i) ) 
    if ( v .lt. 0 ) call inc () 
  end do

  write (6,'("Number of negative v:",1I10)') icount
  stop
end program
