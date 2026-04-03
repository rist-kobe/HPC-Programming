! Copyright 2024 Research Organization for Information Science and Technology
!--------------------------------------------------------------------------
!  Address mapping of 2D array in Fortran 90
!  Author:      Yukihiro Ota (yota@rist.or.jp)
!  Last update: 27 Oct. 2017
!--------------------------------------------------------------------------
program main
  implicit none

  integer,parameter :: nrow = 3
  integer,parameter :: ncol = 8
  integer :: i, j
  integer :: seed_size
#ifndef __ARCH_32__
  integer(8) :: adr_x 
#else
  integer :: adr_x
#endif 
  integer,allocatable :: seed(:)

  real(8) :: xtmp 
  real(8) :: x_s(nrow,ncol)
  real(8),allocatable :: x_d(:,:) 

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
  write (6,'(A)') "Address mapping of 2D array in Fortran 90        "
  write (6,'(A)') "  Author:      Yukihiro Ota (yota@rist.or.jp)    "
  write (6,'(A)') "  Last update: 27 Oct 2017                       " 
  write (6,'(A)') "================================================="

  write (6,'(A)') "Static memory allocation (real(8)=8 bytes)" 
  write (6,'(A)') "x_s(i,j)"
  write (6,'(A)') "row     col     val         address (dec) "
  do j=1,ncol
    do i=1,nrow
      adr_x = loc (x_s(i,j))  !! loc is GNU extensions
      write (6, '(2I5, 1F11.4,1I20)') i, j, x_s(i,j), adr_x 
    enddo
  enddo

  write (6,'(A)') "Dynamical memory allocation (real(8)=8 bytes)" 
  write (6,'(A)') "x_d(i,j)"
  write (6,'(A)') "row     col     val         address (dec)"
  do j=1,ncol
    do i=1,nrow
      adr_x = loc (x_d(i,j))  !! loc is GNU extensions
      write (6, '(2I5, 1F11.4,1I20)') i, j, x_d(i,j), adr_x 
    enddo
  enddo

  ! Finalization
  deallocate ( seed ) ;

  deallocate ( x_d )  

  stop
end program main 
