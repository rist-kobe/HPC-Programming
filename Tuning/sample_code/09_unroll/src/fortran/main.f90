! Copyright 2024 Research Organization for Information Science and Technology
!-----------------------------------------------------------------------
! Loop unrolling on an outer loop
! Author:      Yukihiro Ota (yota@rist.or.jp)
! Description: related to Sec.5.5 in the textbook
!-----------------------------------------------------------------------
program main

  implicit none

  integer :: ierr, i, j, itr, idm
  integer :: nout, nin, ntot
  character(len=32) :: cbuff

  real(8) :: tmp, res 
  real(8),allocatable :: a(:), b(:), aref(:)
  real(8),allocatable :: c(:,:)

#ifdef __USE_CPU_TIMER__
  real(8) :: elpst, elp(3)
#endif

  external fma0, fma4_nf, fma4

  !---------------------------------------------------------------------
  ! get outer and inner loop lengths from arguments
  !---------------------------------------------------------------------
  ierr = command_argument_count ()
  if ( ierr /= 2 ) then
    write (6,"(1a48)") "[usage] ./run.x $1 $2                         "
    write (6,"(1a48)") "   $1: size of outer loop (integer)           "
    write (6,"(1a48)") "   $2: size of inner loop (integer)           "
    stop
  endif
  call get_command_argument (number=1, value=cbuff)
  read(cbuff,"(1i10)") nout
  call get_command_argument (number=2, value=cbuff)
  read(cbuff,"(1i10)") nin
  ntot = nout * nin
  if ( nout < 4 ) then
    write (6,"(1a48)") "Outer loop size must be greager than 4.       "
    write (6,"(1a48)") "The program is terminated.                    "
    stop
  endif
  if ( nin <= 0 ) then
    write (6,"(1a48)") "Inner loop size must be greager than 0.       "
    write (6,"(1a48)") "The program is terminated.                    "
    stop
  endif
  if ( ntot > 8192*8192 ) then
    !! ntot*8 bytes < 512 MiB=2^29 
    write (6,"(1a48)") "(outer loop)*(inner loop) < 8192*8192         "
    write (6,"(1a48)") "The program is terminated.                    "
    stop
  endif
  !---------------------------------------------------------------------
  ! memory allocation
  !---------------------------------------------------------------------
  allocate ( a(nin) ) 
  allocate ( b(nin) )
  allocate ( c(nin,nout) )
  allocate ( aref(nin) )
  !! get physical memory
  tmp = 1.d0/nin
  do i = 1, nin
    a(i) = 0.d0
    b(i) = 1.d0 + (i-nin/2)*tmp
  enddo
  tmp = 1.d0 / ntot
  do j=1, nout
    do i=1, nin
      c(i,j) = 1.d0 + (i+j*nin-ntot/2)*tmp
    enddo
  enddo  
  !---------------------------------------------------------------------
  ! fma0
  !---------------------------------------------------------------------
  idm = 0
#ifdef __USE_CPU_TIMER__
  call cpu_time ( elpst )  
#endif
  do itr = 1, 10000
    call fma0 ( a, b, c, nout, nin, itr, idm )
    idm = idm + 1
  enddo
#ifdef __USE_CPU_TIMER__
  call cpu_time ( elp(1) )
  elp(1) = elp(1) -elpst
  write (6,"(1a48)") "* fma0: original loop                           "
  write (6,"(1a24,1f17.6)") "CPU time (sec) =       ", elp(1)
#endif
  write (6,"(1a24,1i10)")   "Num of iterations =    ", idm 
  aref = a 
  !---------------------------------------------------------------------
  ! fma4_nf
  !---------------------------------------------------------------------
  a = 0.d0
  idm = 0
#ifdef __USE_CPU_TIMER__
  call cpu_time ( elpst )  
#endif
  do itr = 1, 10000
    call fma4_nf ( a, b, c, nout, nin, itr, idm )
    idm = idm + 1
  enddo
#ifdef __USE_CPU_TIMER__
  call cpu_time ( elp(2) )
  elp(2) = elp(2) -elpst
  write (6,"(1a48)") "* fma4_nf: Unrolling, with expanding 4 times,   "
  write (6,"(1a48)") "        but no loop fusion.                     "
  write (6,"(1a24,1f17.6)") "CPU time (sec) =       ", elp(2)
#endif
  write (6,"(1a24,1i10)")   "Num of iterations =    ", idm 
  a = a - aref
  res = 0.d0
  do i = 1, nin
    res = res + a(i)*a(i)
  enddo
  res = sqrt(res)
  write (6,"(1a48)") "Diff. on output b/w (fma4_nf) and (fma0)        "
  write (6,"(1e23.12)") res
  !---------------------------------------------------------------------
  ! fma4
  !---------------------------------------------------------------------
  a = 0.d0
  idm = 0
#ifdef __USE_CPU_TIMER__
  call cpu_time ( elpst )  
#endif
  do itr = 1, 10000
    call fma4 ( a, b, c, nout, nin, itr, idm )
    idm = idm + 1
  enddo
#ifdef __USE_CPU_TIMER__
  call cpu_time ( elp(3) )
  elp(3) = elp(3) -elpst
  write (6,"(1a48)") "* fma4: Unrolling, with expanding 4 times,      "
  write (6,"(1a48)") "        and loop fusion.                        "
  write (6,"(1a24,1f17.6)") "CPU time (sec) =       ", elp(3)
#endif
  write (6,"(1a24,1i10)")   "Num of iterations =    ", idm 
  a = a - aref
  res = 0.d0
  do i = 1, nin
    res = res + a(i)*a(i)
  enddo
  res = sqrt(res)
  write (6,"(1a48)") "Diff. on output b/w (fma4) and (fma0)           "
  write (6,"(1e23.12)") res
  !---------------------------------------------------------------------
  ! Finalization
  !---------------------------------------------------------------------
  deallocate ( a, b, aref ) 
  deallocate ( c ) 
  stop
end program
!-----------------------------------------------------------------------
! fma0
!-----------------------------------------------------------------------
subroutine fma0 ( a, b, c, nout, nin, itr, idm ) 

  implicit none

  real(8),intent(inout) :: a(nin)

  integer,intent(in) :: nout, nin ,itr
  real(8),intent(in) :: b(nin), c(nin,nout)

  integer,intent(out) :: idm
  !---------------------------------------------------------------------
  ! local variables
  !---------------------------------------------------------------------
  integer :: i, j
  !---------------------------------------------------------------------
  ! standard 
  !---------------------------------------------------------------------
  do j = 1, nout
    do i = 1, nin
      a(i) = a(i) + b(i)*c(i,j)
    enddo
  enddo

  idm = itr - 1
end subroutine fma0
!-----------------------------------------------------------------------
! fma4_nf (loop unrolling expanding 4 times in outer loop, 
!          but no loop fusion)
!-----------------------------------------------------------------------
subroutine fma4_nf ( a, b, c, nout, nin, itr, idm ) 

  implicit none

  real(8),intent(inout) :: a(nin)

  integer,intent(in) :: nout, nin ,itr
  real(8),intent(in) :: b(nin), c(nin,nout)

  integer,intent(out) :: idm
  !---------------------------------------------------------------------
  ! local variables
  !---------------------------------------------------------------------
  integer :: i, j, nres

  nres = nout - 4*(nout/4)
  !---------------------------------------------------------------------
  ! unrolling 
  !---------------------------------------------------------------------
  do j = 1, nout-nres, 4

    !! increase statements
    do i = 1, nin
      a(i) = a(i) + b(i)*c(i,j)
    enddo
    do i = 1, nin
      a(i) = a(i) + b(i)*c(i,j+1)
    enddo
    do i = 1, nin
      a(i) = a(i) + b(i)*c(i,j+2)
    enddo
    do i = 1, nin
      a(i) = a(i) + b(i)*c(i,j+3)
    enddo

  enddo
  !---------------------------------------------------------------------
  ! residue
  !---------------------------------------------------------------------
  do j = nout-nres+1, nout
    do i = 1, nin
      a(i) = a(i) + b(i)*c(i,j)
    enddo
  enddo

  idm = itr - 1
end subroutine fma4_nf
!-----------------------------------------------------------------------
! fma4 (loop unrolling with 4 unrolling factors in outer loop,
!       and loop fusion, i.e., unroll and jam)
!-----------------------------------------------------------------------
subroutine fma4 ( a, b, c, nout, nin, itr, idm ) 

  implicit none

  real(8),intent(inout) :: a(nin)

  integer,intent(in) :: nout, nin ,itr
  real(8),intent(in) :: b(nin), c(nin,nout)

  integer,intent(out) :: idm
  !---------------------------------------------------------------------
  ! local variables
  !---------------------------------------------------------------------
  integer :: i, j, nres

  nres = nout - 4*(nout/4)
  !---------------------------------------------------------------------
  ! unrolling 
  !---------------------------------------------------------------------
  do j = 1, nout-nres, 4

    !! increase statements
    do i = 1, nin
      a(i) = a(i) + b(i)*c(i,j)
      a(i) = a(i) + b(i)*c(i,j+1)
      a(i) = a(i) + b(i)*c(i,j+2)
      a(i) = a(i) + b(i)*c(i,j+3)
    enddo

  enddo
  !---------------------------------------------------------------------
  ! residue
  !---------------------------------------------------------------------
  do j = nout-nres+1, nout
    do i = 1, nin
      a(i) = a(i) + b(i)*c(i,j)
    enddo
  enddo

  idm = itr - 1
end subroutine fma4
