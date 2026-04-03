! Copyright 2024 Research Organization for Information Science and Technology
!-----------------------------------------------------------------------
!  Loop blocking: Matrix transpose 
!  Author:      Yukihiro Ota (yota@rist.or.jp)
!  Reference:   See Sec.3.4 in Introduction to High Performance 
!               Computing for Scientists and Engineers by 
!               Hager and Wellein (CRC Press, 2011).
!-----------------------------------------------------------------------
program main
  implicit none

  integer :: nrow, ncol, nbk, nsize, nrbk, ncbk
  integer :: ierr, hnsize, idm 
  integer :: i, j, itr

  character(len=32) :: cbuff

  real(8) :: wk, mema
  real(8),allocatable :: a(:,:), b(:,:)

#ifdef __USE_CPU_TIMER__
  real(8) :: elpsta, elp(2) 
#endif

  external mattp, mattp_bk 

  !---------------------------------------------------------------------
  !  get matrix row size and block size from arguments 
  !---------------------------------------------------------------------
  ierr = command_argument_count ()
  if ( ierr /= 2 ) then 
    write (6,'(1a48)') "[usage] ./run.x $1 $2                          " 
    write (6,'(1a48)') "       $1: matrix row size (integer)           "
    write (6,'(1a48)') "       $2: block size      (integer)           "
    stop
  endif
  !! * argument 1
  call get_command_argument ( number=1, value=cbuff )
  read (cbuff,'(1i10)') nrow
  ncol = nrow
  if ( nrow < 0 ) then
    write (6,'(1a48)') "Matrix row size is greater than 0              "
    write (6,'(1a48)') "The program is terminated.                     "
    stop
  endif
  if ( nrow > 8192 ) then
    !! 2 double arrays * 8 bytes * (nrow)^2 < 1 GiB=2^30 bytes */
    write (6,'(1a48)') "Matrix row size must be smaller than 8192.     "
    write (6,'(1a48)') "The program is terminated.                     "
    stop
  endif
  !! * argument 2
  call get_command_argument ( number=2, value=cbuff )
  read (cbuff,'(1i10)') nbk 
  if ( nbk <= 0 ) then
    write (6,'(1a48)') "Block size must be strictly positive.          "
    write (6,'(1a48)') "The program is terminated.                     "
    stop
  endif
  if ( nbk >= nrow ) then
    write (6,'(1a48)') "Block size must be smaller than matrix row size"
    write (6,'(1a48)') "The program is terminated.                     "
    stop
  endif
  nrbk = (nrow + (nbk-1) ) /nbk !! block num. (rounded up)
  ncbk = (ncol + (nbk-1) ) /nbk !! block num. (rounded up)
  !---------------------------------------------------------------------
  !  memory allocation (and obtain physical memory) 
  !---------------------------------------------------------------------
  nsize = nrow * ncol
  allocate ( a(nrow,ncol) )
  allocate ( b(nrow,ncol) )
  wk = 1.d0 / nsize
  hnsize = nsize / 2
  do j = 1, ncol
    do i = 1, nrow
      a(i,j) = 0.d0
      b(i,j) = 1.d0 + (i+j*nrow-hnsize)*wk 
    enddo
  enddo
  !---------------------------------------------------------------------
  !  matrix transpose
  !---------------------------------------------------------------------
  mema = 2.d0*8.d0*nsize*1.0d-6 !! MB
  idm = 0
#ifdef __USE_CPU_TIMER__
  call cpu_time ( elpsta ) 
#endif 
  do itr = 1, 10000
    call mattp ( b, a, nrow, ncol, itr, idm )
    idm = idm + 1
  enddo
#ifdef __USE_CPU_TIMER__
  call cpu_time ( elp(1) )
  elp(1) = elp(1) - elpsta
#endif
  !---------------------------------------------------------------------
  !  standard output 
  !---------------------------------------------------------------------
  write (6,'(1a48)') "Matrix transpose (standard)                      "
  write (6,'(1a48)') "Number of iterations                             "
  write (6,'(1i10)') idm
  write (6,'(1a48)') "Check matrix transpose                           "
  write (6,'(2i5,2f17.6)') 1, ncol, a(1,ncol), b(1,ncol)
  write (6,'(2i5,2f17.6)') nrow, 1, a(nrow,1), b(nrow,1)
  write (6,'(1a48)') "Req. memory (MB)                                 "
  write (6,'(1f17.6)') mema
#ifdef __USE_CPU_TIMER__
  write (6,'(1a48)') "CPU time (sec)                                   "
  write (6,'(1f17.6)') elp(1) 
#endif
  !---------------------------------------------------------------------
  !  memory deallocation and allocation (and obtain physical memory) 
  !---------------------------------------------------------------------
  deallocate ( a )
  deallocate ( b )
  allocate ( a(nrow,ncol) )
  allocate ( b(nrow,ncol) )
  wk = 1.d0 / nsize
  hnsize = nsize / 2
  do j = 1, ncol
    do i = 1, nrow
      a(i,j) = 0.d0
      b(i,j) = 1.d0 + (i+j*nrow-hnsize)*wk 
    enddo
  enddo
  !---------------------------------------------------------------------
  !  matrix transpose  (blocking)
  !---------------------------------------------------------------------
  mema = 2.d0*8.d0*nsize*1.0d-6 !! MB
  idm = 0
#ifdef __USE_CPU_TIMER__
  call cpu_time ( elpsta ) 
#endif 
  do itr = 1, 10000
    call mattp_bk ( b, a, nrow, ncol, itr, idm, nbk )
    idm = idm + 1
  enddo
#ifdef __USE_CPU_TIMER__
  call cpu_time ( elp(2) )
  elp(2) = elp(2) - elpsta
#endif
  !---------------------------------------------------------------------
  !  standard output 
  !---------------------------------------------------------------------
  write (6,'(1a48)') "Matrix transpose (blocking)                      "
  write (6,'(1a48)') "Number of iterations                             "
  write (6,'(1i10)') idm
  write (6,'(1a48)') "Check matrix transpose                           "
  write (6,'(2i5,2f17.6)') 1, ncol, a(1,ncol), b(1,ncol)
  write (6,'(2i5,2f17.6)') nrow, 1, a(nrow,1), b(nrow,1)
  write (6,'(1a48)') "Req. memory (MB)                                 "
  write (6,'(1f17.6)') mema
  write (6,'(1a48)') "Number of blocks w.r.t. row and column           "
  write (6,'(4x,1i5,2x,1i5)') nrbk, ncbk
#ifdef __USE_CPU_TIMER__
  write (6,'(1a48)') "CPU time (sec)                                   "
  write (6,'(1f17.6)') elp(2) 
#endif
  !---------------------------------------------------------------------
  !  Finalize 
  !---------------------------------------------------------------------
  deallocate ( a, b ) 
  stop
end 
!-----------------------------------------------------------------------
!  mattp 
!-----------------------------------------------------------------------
subroutine mattp ( a, a_tp, nr, nc, it, idm )
  implicit none

  integer,intent(in) :: nr, nc, it
  real(8),intent(in) :: a(nr,nc)

  integer,intent(out) :: idm
  real(8),intent(out) :: a_tp(nr,nc)
  !---------------------------------------------------------------------
  ! * local variables
  !---------------------------------------------------------------------
  integer :: i, j
  !---------------------------------------------------------------------
  ! * main routine 
  !---------------------------------------------------------------------
  do j = 1, nc
  do i = 1, nr

    a_tp(i,j) = a(j,i) !! 1 load, 1 sotre, no FP operation

  enddo
  enddo

  idm = it - 1

  return
end subroutine
!-----------------------------------------------------------------------
!  blocked mattp 
!-----------------------------------------------------------------------
subroutine mattp_bk ( a, a_tp, nr, nc, it, idm, nbk )
  implicit none

  integer,intent(in) :: nr, nc, it, nbk
  real(8),intent(in) :: a(nr,nc)

  integer,intent(out) :: idm
  real(8),intent(out) :: a_tp(nr,nc)
  !---------------------------------------------------------------------
  ! * local variables
  !---------------------------------------------------------------------
  integer :: i, j, ii, jj
  !---------------------------------------------------------------------
  ! * main routine 
  !---------------------------------------------------------------------
  do jj = 1, nc, nbk
  do ii = 1, nr, nbk

    do j = jj, min(jj+nbk-1,nc)
    do i = ii, min(ii+nbk-1,nr)
      a_tp(i,j) = a(j,i)  !! 1 load, 1 store, no FP operation
    enddo
    enddo

  enddo
  enddo

  idm = it - 1

  return
end subroutine
