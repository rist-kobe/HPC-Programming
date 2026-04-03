! Copyright 2024 Research Organization for Information Science and Technology
!----------------------------------------------------------------------
!  Thrashing
!  Author:       Yukihiro Ota (yota@rist.or.jp)
!  Last update:  23rd Jan., 2024
!  Usage:        run.x (arg1)
!                  (arg1) = size of vectors (integer) = nsize
!                When nsize = (integer)*(Size of L1 D cache per way)/8bytes,
!                thrasing may occur. Examine elapsed time around 
!                this target value of nsize.
!  Note 1:       Before running this, check size of L1 data cache per 
!                way in your computer. On Unix/Linux, use of getconf
!                command is a practical option for this purpose.
!  Note 2:       If cache associativity (i.e., number of ways) is 
!                larger, an effect of thrashing will be less.  
!-----------------------------------------------------------------------
module mytype
  implicit none
  integer,parameter :: SP = kind(1.0)
  integer,parameter :: DP = selected_real_kind(2*precision(1.0_SP))
end module mytype
!-----------------------------------------------------------------------
module mykernel
  use mytype,only: DP

contains
  subroutine kernel_l3_s1(icon, ns, u, a, b, c)
    integer,intent(inout) :: icon
    integer,intent(in) :: ns
    real(kind=DP),intent(out) :: u(1:ns)
    real(kind=DP),intent(in) :: a(1:ns)
    real(kind=DP),intent(in) :: b(1:ns)
    real(kind=DP),intent(in) :: c(1:ns)

    integer :: i
    ! 3 loads, 1 store
    do i = 1, ns
      u(i) = a(i) + b(i)*c(i)
    end do

    icon = icon + 1
  end subroutine kernel_l3_s1
  !
  subroutine kernel_l4_ls1(icon, ns, u, a, b, c, d)
    integer,intent(inout) :: icon
    integer,intent(in) :: ns
    real(kind=DP),intent(inout) :: u(1:ns)
    real(kind=DP),intent(in) :: a(1:ns)
    real(kind=DP),intent(in) :: b(1:ns)
    real(kind=DP),intent(in) :: c(1:ns)
    real(kind=DP),intent(in) :: d(1:ns)

    integer :: i
    ! 4 loads, 1 load and store
    do i = 1, ns
      u(i) = u(i) + d(i)*(a(i) + b(i)*c(i))
    end do

    icon = icon + 1
  end subroutine kernel_l4_ls1
  !
  subroutine kernel_l8_ls1(icon, ns, u, a, b, c, d, e, f, g, h)
    integer,intent(inout) :: icon
    integer,intent(in) :: ns
    real(kind=DP),intent(inout) :: u(1:ns)
    real(kind=DP),intent(in) :: a(1:ns)
    real(kind=DP),intent(in) :: b(1:ns)
    real(kind=DP),intent(in) :: c(1:ns)
    real(kind=DP),intent(in) :: d(1:ns)
    real(kind=DP),intent(in) :: e(1:ns)
    real(kind=DP),intent(in) :: f(1:ns)
    real(kind=DP),intent(in) :: g(1:ns)
    real(kind=DP),intent(in) :: h(1:ns)

    integer :: i
    real(kind=DP) :: u1, u2
    ! 8 loads, 1 load and store
    do i = 1, ns
      u1 = d(i)*(a(i) + b(i)*c(i))
      u2 = h(i)*(e(i) + f(i)*g(i))
      u(i) = u(i) + u1 + u2
    end do

    icon = icon + 1
  end subroutine kernel_l8_ls1
end module mykernel
!-----------------------------------------------------------------------
program main
  use mytype,only: DP
  use mykernel,only: kernel_l4_ls1, kernel_l8_ls1
  implicit none
  
  integer :: i, itr, icon
  integer :: narg
  integer :: nsize
  character(len=256) :: cbuf

  real(kind=DP) :: cpu1, cpu21, cpu22
  real(kind=DP) :: memuse
  real(kind=DP),allocatable :: U(:)
  real(kind=DP),allocatable :: A(:)
  real(kind=DP),allocatable :: B(:) 
  real(kind=DP),allocatable :: C(:) 
  real(kind=DP),allocatable :: D(:) 
  real(kind=DP),allocatable :: E(:)
  real(kind=DP),allocatable :: F(:) 
  real(kind=DP),allocatable :: G(:) 
  real(kind=DP),allocatable :: H(:) 

  !! parse command-line option 
  narg = command_argument_count() 
  if ( narg .ne. 1 ) then
    write(6,'(1A)') "[usage] run.x (arg1)"
    write(6,'(1A)') "  arg1: vector size (int)"
    stop
  end if

  call get_command_argument(1, cbuf)
  read(cbuf,'(1I10)') nsize

  !! memory allocation
  allocate( U(1:nsize) )
  allocate( A(1:nsize) )
  allocate( B(1:nsize) )
  allocate( C(1:nsize) )
  allocate( D(1:nsize) )
  allocate( E(1:nsize) )
  allocate( F(1:nsize) )
  allocate( G(1:nsize) )
  allocate( H(1:nsize) )

  do i = 1, nsize
    U(i) = 0.0_DP
    A(i) = 1.0_DP
    B(i) = 2.0_DP
    C(i) = 3.0_DP
    D(i) = 4.0_DP
    E(i) = 1.0_DP
    F(i) = 2.0_DP
    G(i) = 3.0_DP
    H(i) = 4.0_DP
  end do

  memuse = 0.0_DP
  memuse = memuse + DP*nsize
  memuse = memuse / 1024

  !! main calculation
  icon = 0
  call cpu_time( cpu1 )
  do itr = 1, 10000
    ! 4 loads, 1 load and store
    call kernel_l4_ls1 (icon, nsize, U, A, B, C, D)
    icon = mod(itr, 2)
  end do
  call cpu_time( cpu21 )
  cpu21 = cpu21 - cpu1

  icon = 0
  call cpu_time( cpu1 )
  do itr = 1, 10000
    ! 4 loads, 1 load and store
    call kernel_l8_ls1 (icon, nsize, U, A, B, C, D, E, F, G, H)
    icon = mod(itr, 2)
  end do
  call cpu_time( cpu22 )
  cpu22 = cpu22 - cpu1

  !! output
  write(6,'("nsize",1I10)') nsize
  write(6,'("# kernel")')
  write(6,'("kernel_l4_ls1: 4 loads, 1 load and store.")')
  write(6,'("  Thrashing may occur when associativity is &
& less than and equal to 4.")')
  write(6,'("kernel_l8_ls1: 8 loads, 1 load and store.")')
  write(6,'("  Thrashing may occur when associativity is &
& less than and equal to 8.")')
  write(6,'("# output")')

  open(10,file='logfile',access='append') 
  write(10,'(1I10,1F10.4,2F9.3,2I10,1F11.4)') &
    nsize,memuse,cpu21,cpu22,5,9,U(1)
  close(10)

  !!! GNU extension function, loc, is used. If you do not want
  !!! to use it, comment out the following lines.
#if ! defined(NOT_USE_GNULOC)
  open(20,file='add.info',access='append')
  write(20,'("nsize=",1I10)') nsize
  write(20,'(2I10)') loc(U(1)),loc(U(nsize))
  write(20,'(2I10)') loc(A(1)),loc(A(nsize))
  write(20,'(2I10)') loc(B(1)),loc(B(nsize))
  write(20,'(2I10)') loc(C(1)),loc(C(nsize))
  write(20,'(2I10)') loc(D(1)),loc(D(nsize))
  write(20,'(2I10)') loc(E(1)),loc(E(nsize))
  write(20,'(2I10)') loc(F(1)),loc(F(nsize))
  write(20,'(2I10)') loc(G(1)),loc(G(nsize))
  write(20,'(2I10)') loc(H(1)),loc(H(nsize))
  write(20,'("--------------------")')
  close(20)
#endif

  !! finalization
  deallocate( A )
  deallocate( B )
  deallocate( C )
  deallocate( D )

  stop
end program main
