! Copyright 2024 Research Organization for Information Science and Technology
!----------------------------------------------------------------------
!  Strip mining
!  Author:       Yukihiro Ota (yota@rist.or.jp)
!  Last update:  28 Aug 2018
!----------------------------------------------------------------------
program main
  use ISO_C_BINDING
  implicit none
  !---------------------------------------------------------------------
  !   local variables
  !---------------------------------------------------------------------
  integer :: i, itr
  integer :: narg
  integer :: nlarge, nsmall, veclen
  integer,parameter :: totitr = 10000000
  character(len=256) :: cbuf

  real(c_double) stmp1, stmp2
  real(8),allocatable :: A(:)
  real(8) :: B

  external addmulti, addmulti_stripmining

  !---------------------------------------------------------------------
  interface
  !---------------------------------------------------------------------
  real(c_double) function get_elp_timer() bind(c)
    use ISO_C_BINDING,only: c_double
  end function get_elp_timer

  real(c_double) function get_cpu_time() bind(c)
    use ISO_C_BINDING,only: c_double
  end function get_cpu_time
  !---------------------------------------------------------------------
  end interface
  !---------------------------------------------------------------------

  !---------------------------------------------------------------------
  !   parse command line options 
  !---------------------------------------------------------------------
  narg = command_argument_count() 
  if ( narg .ne. 3 ) then
    write(6,'(1A)') "[usage] run.x (arg1) (arg2) (arg3)"
    write(6,'(1A)') "   arg1: 1st loop length (integer)"
    write(6,'(1A)') "   arg2: 2nd loop length (integer)"
    write(6,'(1A)') "   arg3: vector length of strip-mining (integer)"
    write(6,'(1A)') "   requirement: (arg2) < (arg1)"
    write(6,'(1A)') "   requirement: (arg3) < (arg2)"
    stop
  end if 

  call get_command_argument(1,cbuf)
  read(cbuf,'(1I10)') nlarge 

  call get_command_argument(2,cbuf)
  read(cbuf,'(1I10)') nsmall
  if ( nlarge < nsmall ) then
    write(6,'(1A)') "Error: 2nd arg must be smaller than 1st one."
    stop
  end if

  call get_command_argument(3,cbuf)
  read(cbuf,'(1I10)') veclen
  if ( nsmall < veclen ) then
    write(6,'(1A)') "Error: 3rd arg must be smaller than 2nd one."
    stop
  end if

  write(6,'(1A)') "[Job setting]"
  write(6,'("1st loop length",1I10)') nlarge
  write(6,'("2nd loop length",1I10)') nsmall
  write(6,'("vec length of strip-mining",1I10)') veclen 

  !---------------------------------------------------------------------
  !   Addition loops and Multiplication loop 
  !---------------------------------------------------------------------
  allocate( A(1:nlarge) )

  do i = 1, nlarge
    A(i) = dble(i-nlarge/2)*1.0D0/nlarge
  end do

  B = 0.0D0

  stmp1 = get_cpu_time()
  do itr = 1, totitr 
    call addmulti(A,B,nlarge,nsmall)
    B = dble(itr)/totitr
  end do
  stmp2 = get_cpu_time()

  write(6,'(1A)') "Standard"
  write(6,'("CPU time (sec)",1F13.6)') stmp2 - stmp1
  write(6,'("A(1) = ",1E15.6)') A(1)

  deallocate(A) 
  !---------------------------------------------------------------------
  !   Strip minging: Addition loops and Multiplication loop 
  !---------------------------------------------------------------------
  allocate( A(1:nlarge) )

  do i = 1, nlarge
    A(i) = dble(i-nlarge/2)*1.0D0/nlarge
  end do

  B = 0.0D0

  stmp1 = get_cpu_time()
  do itr = 1, totitr
    call addmulti_stripmining(A,B,nlarge,nsmall,veclen)
    B = dble(itr)/totitr
  end do
  stmp2 = get_cpu_time()

  write(6,'(1A,1I10)') "Strip-mining with vector length", veclen
  write(6,'("CPU time (sec)",1F13.6)') stmp2 - stmp1
  write(6,'("A(1) = ",1E15.6)') A(1)

  deallocate(A) 
  !---------------------------------------------------------------------
  !   Finalization 
  !---------------------------------------------------------------------
  stop
end program
!---------------------------------------------------------------------
!   Addition loops and Multiplication loop 
!---------------------------------------------------------------------
subroutine addmulti(A, B, NL, NS)
  implicit none 
  real(8), intent(inout) :: A(NL)
  real(8), intent(in) :: B
  integer, intent(in) :: NL, NS

  integer :: i

  do i = 1, NL
    A(i) = A(i) + B
  end do

  do i = 1, NS
    A(i) = A(i) * B
  end do 
end subroutine addmulti
!---------------------------------------------------------------------
!   Strip mining: Addition loops and Multiplication loop 
!---------------------------------------------------------------------
subroutine addmulti_stripmining(A, B, NL, NS, VLEN)
  implicit none 
  real(8), intent(inout) :: A(NL)
  real(8), intent(in) :: B
  integer, intent(in) :: NL, NS, VLEN

  integer :: i, ii

  do i = 1, NL, VLEN

    !! if VLEN elements are on cache, the data is re-usable
    !! in the two successive procedures (add and multi).
    do ii = i, MIN(i+VLEN-1,NL)
      A(ii) = A(ii) + B
    end do
    do ii = i, MIN(i+VLEN-1,NS)
      A(ii) = A(ii) * B
    end do

  end do
end subroutine addmulti_stripmining
