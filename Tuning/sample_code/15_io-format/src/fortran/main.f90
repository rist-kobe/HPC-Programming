! Copyright 2024 Research Organization for Information Science and Technology
program main
  use mytype, only: SP, DP
  use timer, only: get_elp_time, get_cpu_time
  implicit none

  integer,parameter :: iunit = 11 
  integer,parameter :: NX=1024 !!2^10
  integer,parameter :: NY=1024 !!2^10
  integer :: NZ
  integer :: narg, ifmt
  integer :: i, j, k
  integer :: ielp0
  integer,allocatable,dimension(:,:,:) :: AA

  character(len=64) :: cbuf

  real(kind=DP) :: elp, elp0, cpu 
  real(kind=DP) :: aamemsize 

  !!write(6,*) SP, DP 

  narg = command_argument_count()
  if ( narg .ne. 2 ) then
     write(6,'("[usage] ./run.x (arg1) (arg2)",/,    &
&              "   (arg1): Integer (0-1)",/,         &
&              "           0: write w/  format",/,   &
&              "           1: write w/o format",/,   &
&              "   (arg2): Integer (<200) to change array size" &
&              )')
     stop
  end if

  call get_command_argument(1,cbuf)
  read(cbuf,'(1I10)') ifmt
  call get_command_argument(2,cbuf)
  read(cbuf,'(1I10)') NZ

  if ( NZ .gt. 200 ) then
     write(6,'(1A)') "[Error] 2nd arg must be < 200."
     stop 
  end if

  allocate( AA(NX,NY,NZ) ) 

  do k = 1, NZ
  do j = 1, NY
  do i = 1, NX
     AA(i,j,k) = i-1 + (j-1 + (k-1)*NY)*NX 
  end do
  end do
  end do

  aamemsize = 4.0_DP*NX*NY*NZ / 1048576 ! MiB

  !-------------------------------------------------------------------
  ! write with format
  !-------------------------------------------------------------------
  if ( ifmt .eq. 0 ) then

  cbuf="data1.out"
  open(iunit,FILE=cbuf) 
  call get_elp_time(ioffset=ielp0)  
  do k = 1, NZ
  do j = 1, NY
  do i = 1, NX
     write(iunit,'(1I10)') AA(i,j,k)
  end do
  end do
  end do
  call get_elp_time(ioffset=ielp0,tv=elp)
  close(iunit)

  cbuf="data2.out"
  open(iunit,FILE=cbuf) 
  elp0 = get_cpu_time()  
  do k = 1, NZ
  do j = 1, NY
  do i = 1, NX
     write(iunit,'(1I10)') AA(i,j,k)
  end do
  end do
  end do
  cpu = get_cpu_time() - elp0 
  close(iunit)

  !-------------------------------------------------------------------
  ! write with unformat
  !-------------------------------------------------------------------
  else if ( ifmt .eq. 1 ) then

  cbuf="data1.out"
  open(iunit,FILE=cbuf,FORM='unformatted',ACCESS='stream') 
  call get_elp_time(ioffset=ielp0)  
  write(iunit) AA
  call get_elp_time(ioffset=ielp0,tv=elp)
  close(iunit)
 
  cbuf="data2.out"
  open(iunit,FILE=cbuf,FORM='unformatted',ACCESS='stream') 
  elp0 = get_cpu_time()  
  write(iunit) AA
  cpu = get_cpu_time() - elp0
  close(iunit)

  !-------------------------------------------------------------------
  ! others 
  !-------------------------------------------------------------------
  else
 
  write(6,'(5x,1A)') "[Error] No routine. Check command-line options."
  deallocate( AA )
  stop

  end if

  write(6,'(1A)') "format memory_MiB elapsed_time_sec cpu_time_sec diff"
  write(6,'(1I3,1F11.4,1F11.4,1F11.4,1F11.4)') &
&   ifmt, aamemsize, elp, cpu, elp-cpu

  deallocate( AA )

  stop
end program
