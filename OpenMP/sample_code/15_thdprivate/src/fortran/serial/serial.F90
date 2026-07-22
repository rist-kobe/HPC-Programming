! Copyright 2024 Research Organization for Information Science and Technology
module myvar
  integer icount

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
  use myvar,only: icount, inc, func
  implicit none
  integer :: i
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

  icount = 0

  do i = 1, nsize
    v = func ( a(i) ) 
    if ( v .lt. 0 ) call inc () 
  end do

  write (6,'("Number of negative v:",1I10)') icount

end program main
