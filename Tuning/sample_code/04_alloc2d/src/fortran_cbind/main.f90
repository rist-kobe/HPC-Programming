! Copyright 2024 Research Organization for Information Science and Technology
!--------------------------------------------------------------------------
!  Address mapping of 2D array in Fortran2003 with ISO_C_BINDING 
!  Author:      Yukihiro Ota (yota@rist.or.jp)
!  Last update: 29 Jan. 2018
!  Reference:   M. Metcalf et al. "Modern Fortran explained"
!               (Oxford, 2011) Chap.12
!  Comment:     I'd like to thank an audience in the Lecture at 18 Jan. 
!               2018 for a useful comment. 
!--------------------------------------------------------------------------
program main
  !! iso_c_binding is available for Fortran2003 and later 
  use, intrinsic::ISO_C_BINDING,only: c_double, c_ptr, c_loc 
  implicit none

  integer,parameter :: nrow = 3
  integer,parameter :: ncol = 8
  integer :: i, j
  integer :: seed_size
  integer,allocatable :: seed(:)

  real(8) :: xtmp 
  real(kind=c_double), target :: x_s(nrow,ncol)
  real(kind=c_double), allocatable, target :: x_d(:,:) 

  type(c_ptr) :: adrs 

  interface
    !! wrapper.c
    subroutine printf_wrapper(i, j, val, cptr) bind(c,name='printf_wrapper') 
      use, intrinsic::ISO_C_BINDING,only: c_int, c_double, c_ptr
      implicit none
      integer(kind=c_int), value :: i, j
      real(kind=c_double), value :: val
      type(c_ptr), value :: cptr
    end subroutine printf_wrapper
  end interface

  allocate ( x_d(1:nrow,1:ncol) )
  !------------------------------------------------------------------------
  ! initialization by uniform random number in [0,1) 
  !------------------------------------------------------------------------
  call random_seed (size=seed_size) 

  allocate( seed(seed_size) )
  
  call random_seed (get=seed) 

  call system_clock (count=i)
  do i=1,seed_size
    call system_clock (count=seed(i))
  enddo

  call random_seed (put=seed)

  do j=1,ncol
  do i=1,nrow
     call random_number (xtmp)
     x_s(i,j) = xtmp 
     x_d(i,j) = xtmp 
  enddo
  enddo
  !------------------------------------------------------------------------
  ! main calculation
  !------------------------------------------------------------------------
  write (6,'(A)') "================================================="
  write (6,'(A)') "Address mapping of 2D array in Fortran 2003      "
  write (6,'(A)') "with ISO_C_BINDING                               "
  write (6,'(A)') "  Author:      Yukihiro Ota (yota@rist.or.jp)    "
  write (6,'(A)') "  Last update: 29 Jan.2018                       " 
  write (6,'(A)') "  Ref.:        M. Metcalf et al. Modern Fortran  " 
  write (6,'(A)') "               explained (Oxford) Chap.12        " 
  write (6,'(A)') "================================================="

  write (6,'(A)') "Static memory allocation (real(c_double)=8 bytes)" 
  write (6,'(A)') "x_s(i,j)"
  write (6,'(A)') "row     col     val         address (hex) "
  do j=1,ncol
  do i=1,nrow
     adrs = c_loc(x_s(i,j))
#ifdef __USE_GNU__
     !! For GNU compiler, direct use of write/print would work
     write (6, '(2I5, 1F11.4, 1z20)') i, j, x_s(i,j), adrs 
#else
     call printf_wrapper( i, j, x_s(i,j), adrs )
#endif
  enddo
  enddo

  write (6,'(A)') "Dynamical memory allocation (real(c_double)=8 bytes)" 
  write (6,'(A)') "x_d(i,j)"
  write (6,'(A)') "row     col     val         address (hex)"
  do j=1,ncol
  do i=1,nrow
     adrs = c_loc(x_d(i,j))
#ifdef __USE_GNU__
     !! For GNU compiler, direct use of write/print would work
     write (6, '(2I5, 1F11.4, 1z20)') i, j, x_d(i,j), adrs 
#else
     call printf_wrapper( i, j, x_d(i,j), adrs )
#endif
  enddo
  enddo

  ! Finalization
  deallocate ( seed ) ;

  deallocate ( x_d )  

  stop
end program main 