! Copyright 2026 Research Organization for Information Science and Technology
! /*--------------------------------------------------------------------
!  Title:       Jacobi method (2-dim. model)
!  Author:      Yukihiro Ota (yota@rist.or.jp)
!  Last update: July 22nd, 2026
!  Reference:   
!    [1] M. Sugihara and K. Murota, "Theoretical Numerical Linear 
!    Algebra" (Iwanami,2009) [in Japanese].
!    [2] Lis; https://www.ssisc.org/lis/index.ex.html
! --------------------------------------------------------------------*/
program main
  use mytype,only:DP
  use omp_lib
  implicit none

  integer,parameter :: NX = 1024 
  integer,parameter :: NY = 1024 
  integer,parameter :: MAXITR = 1000
  integer :: itr, ix, iy
  integer :: iunit

  logical :: lconv

  real(kind=DP),parameter :: tol = 1.0E-3_DP
  real(kind=DP),parameter :: chg = 1.0_DP
  real(kind=DP) :: nrmsq
  real(kind=DP) :: nrmbsq
  real(kind=DP) :: elp0
  real(kind=DP) :: elp(1:3)
  ! Double buffering: PHIE holds the current iterate and PHIO the
  ! next one; after each sweep the new solution is copied back to
  ! PHIE.
#if defined(USE_STATICMEM)
  ! For static memory. NOTE: about 24 MiB is placed on the stack;
  ! run "ulimit -s unlimited" beforehand.
  real(kind=DP) :: PHIE(1:NX,1:NY)
  real(kind=DP) :: PHIO(1:NX,1:NY)
  real(kind=DP) :: RHO(1:NX,1:NY)
#else
  real(kind=DP),allocatable,dimension(:,:) :: PHIE, PHIO, RHO

  allocate( PHIE(1:NX,1:NY) )
  allocate( PHIO(1:NX,1:NY) )
  allocate( RHO(1:NX,1:NY)  )
#endif
  elp0 = omp_get_wtime()

  ! initialize
  RHO(:,:) = 0.0_DP
  ! A point charge at the grid-center cell (NX/2+1, NY/2+1 in 1-based
  ! indexing; the same cell as NX/2, NY/2 in the 0-based C version).
  RHO(NX/2+1,NY/2+1) =  chg

  nrmbsq = 0.0_DP
!$OMP parallel do schedule(static) default(none) shared(RHO) &
!$OMP & private(ix,iy) reduction(+:nrmbsq)
  do iy = 1, NY
  do ix = 1, NX
     nrmbsq = nrmbsq + RHO(ix,iy) * RHO(ix,iy)
  end do
  end do
!$OMP end parallel do
  
!$OMP parallel do schedule(static) default(none) shared(PHIE,PHIO) &
!$OMP & private(ix,iy)
  do iy = 1, NY
  do ix = 1, NX
    PHIE(ix,iy) = chg * 1.0E-2_DP
    PHIO(ix,iy) = 0.0_DP
  end do
  end do
!$OMP end parallel do

  ! Dirichlet boundary condition: phi = 0 on the whole boundary.
  ! Both buffers get zero boundaries here, so the boundary never
  ! needs to be touched again inside the iteration loop.
!$OMP parallel do schedule(static) default(none) shared(PHIE,PHIO) private(ix)
  do ix = 1, NX
    PHIE(ix,1) = 0.0_DP
    PHIE(ix,NY) = 0.0_DP
    PHIO(ix,1) = 0.0_DP
    PHIO(ix,NY) = 0.0_DP
  end do
!$OMP end parallel do

!$OMP parallel do schedule(static) default(none) shared(PHIE,PHIO) private(iy)
  do iy = 1, NY
    PHIE(1,iy) = 0.0_DP
    PHIE(NX,iy) = 0.0_DP
    PHIO(1,iy) = 0.0_DP
    PHIO(NX,iy) = 0.0_DP
  end do
!$OMP end parallel do

  elp(1) = omp_get_wtime() - elp0

  elp0 = omp_get_wtime()

  ! main loop
  lconv = .false.
  do itr =  1, MAXITR

!$OMP parallel do schedule(static) default(none) shared(PHIE,PHIO,RHO) &
!$OMP & private(ix,iy)
    do iy = 2, NY-1
    do ix = 2, NX-1
      PHIO(ix,iy) = 0.25_DP                           &
&                  * (  PHIE(ix+1,iy) + PHIE(ix-1,iy) &
&                     + PHIE(ix,iy+1) + PHIE(ix,iy-1) &
&                     + RHO(ix,iy))
    end do
    end do
!$OMP end parallel do
    ! The interior sweep never writes boundary cells and the copy
    ! below preserves them, so the zero boundary set during the
    ! initialization remains valid throughout the iteration.

    ! Residual test: || A x - b ||^2 <= tol * || b ||^2. The factor
    ! 16 = 4^2 rescales the difference of the two iterates by the
    ! diagonal entry of the discrete Laplacian. The copy back to
    ! PHIE is fused into the same loop: both statements traverse
    ! the same data, so the fusion halves the memory traffic.
    nrmsq = 0.0_DP
!$OMP parallel do schedule(static) default(none) shared(PHIE,PHIO) &
!$OMP & private(ix,iy) reduction(+:nrmsq)
    do iy = 1, NY
    do ix = 1, NX
      nrmsq = nrmsq + 16.0_DP * ( PHIO(ix,iy) - PHIE (ix,iy) ) &
&                             * ( PHIO(ix,iy) - PHIE (ix,iy) )
      PHIE(ix,iy) = PHIO(ix,iy)
    end do
    end do
!$OMP end parallel do

    if ( tol*nrmbsq >= nrmsq ) then
      lconv = .true.
      exit
    endif 
  end do

  elp(2) = omp_get_wtime() - elp0

  if ( lconv ) then
    write (6,'("Convergence")')
    write (6,'("Itr. count =",1I10)') itr - 1
  else
    write (6,'("Not convergence")')
    write (6,'("Itr. count =",1I10)') itr - 1
  end if

  elp0 = omp_get_wtime()

  ! finalize
  open (newunit=iunit, file='phi.dat', status='replace', action='write')
  do iy = 1, NY
  do ix = 1, NX
    write (iunit, '(2I10,1F13.4)') ix,iy,PHIE(ix,iy)
  end do
    write (iunit, *) 
  end do
  close (iunit)

  elp(3) = omp_get_wtime() - elp0

  write(6, '("init     : ",1F13.4,"sec.")') elp(1)
  write(6, '("main loop: ",1F13.4,"sec.")') elp(2)
  write(6, '("i/o      : ",1F13.4,"sec.")') elp(3)

#if ! defined(USE_STATICMEM)
  deallocate( PHIE )
  deallocate( PHIO )
  deallocate( RHO  )
#endif
end program main
