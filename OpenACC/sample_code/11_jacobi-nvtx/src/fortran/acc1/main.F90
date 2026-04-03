! Copyright 2024 Research Organization for Information Science and Technology
! /*--------------------------------------------------------------------
!  Title:       Jacobi method (2-dim. model, dynamical memory allocation)
!  Author:      Yukihiro Ota (yota@rist.or.jp)
!  Last update: 31st Jan. 2024
!  Reference:   
!    [1] M. Sugihara and K. Murota, "Theoretical Numerical Linear 
!    Algebra" (Iwanami,2009) [in Japanese].
!    [2] Lis; https://www.ssisc.org/lis/index.ex.html
! --------------------------------------------------------------------*/
program main
  use mytype,only:DP
#if defined(USE_NVTX)
  use nvtx
#endif
  implicit none

  include 'omp_lib.h'

  integer,parameter :: NX = 8192 
  integer,parameter :: NY = 8192 
  integer,parameter :: MAXITR = 100
  integer :: itr, ix, iy

  logical :: lconv

  real(kind=DP),parameter :: tol = 1.0E-6_DP
  real(kind=DP),parameter :: chg = 1.0_DP
  real(kind=DP) :: nrmsq
  real(kind=DP) :: nrmbsq
  real(kind=DP) :: elp0
  real(kind=DP) :: elp(1:3)
  real(kind=DP), allocatable, dimension(:,:) :: PHIE, PHIO, RHO

  allocate( PHIE(1:NX,1:NY) )
  allocate( PHIO(1:NX,1:NY) )
  allocate( RHO(1:NX,1:NY)  )

  elp0 = omp_get_wtime()

  ! initilize
  RHO(:,:) = 0.0_DP
  RHO((NX-1)/2,(NY-1)/2) =  chg 

  nrmbsq = chg
  
!$OMP parallel do schedule(static) shared(PHIE,PHIO) private(ix,iy) 
  do iy = 1, NY
  do ix = 1, NX
    PHIE(ix,iy) = chg * 1.0E-2_DP
    PHIO(ix,iy) = 0.0_DP
  end do
  end do
!$OMP end parallel do

!$OMP parallel do schedule(static) shared(PHIE) private(ix)
  do ix = 1, NX
    PHIE(ix,1) = 0.0_DP
    PHIE(ix,NY) = 0.0_DP
  end do
!$OMP end parallel do

!$OMP parallel do schedule(static) shared(PHIE) private(iy)
  do iy = 1, NY
    PHIE(1,iy) = 0.0_DP
    PHIE(NX,iy) = 0.0_DP
  end do
!$OMP end parallel do

  elp(1) = omp_get_wtime() - elp0

  elp0 = omp_get_wtime()

  ! main loop
#if defined(USE_NVTX)
  call nvtxStartRange("rept")
#endif
  lconv = .false.
  do itr =  1, MAXITR

#if defined(USE_NVTX)
    call nvtxStartRange("update")
#endif
    nrmsq = 0.0_DP

    !$ACC data copyin(PHIE(1:NX,1:NY),RHO(1:NX,1:NY)) copyout(PHIO(1:NX,1:NY))
    !$ACC kernels
    !$ACC loop independent collapse(2) reduction(max:nrmsq)
    do iy = 2, NY-1
    do ix = 2, NX-1
      PHIO(ix,iy) = 0.25_DP                           &
&                  * (  PHIE(ix+1,iy) + PHIE(ix-1,iy) &
&                     + PHIE(ix,iy+1) + PHIE(ix,iy-1) &
&                     + RHO(ix,iy))
      nrmsq = max(nrmsq, abs(PHIO(ix,iy) - PHIE(ix,iy)))
    end do
    end do
    !$ACC end kernels
    !$ACC end data
#if defined(USE_NVTX)
    call nvtxEndRange()
#endif
#if 0
    do ix = 1, NX
      PHIO(ix,1) = 0.0_DP
      PHIO(ix,NY) = 0.0_DP
    end do

    do iy = 1, NY
      PHIO(1,iy) = 0.0_DP
      PHIO(NX,iy) = 0.0_DP
    end do
#endif

#if defined(USE_NVTX)
    call nvtxStartRange("swap")
#endif
    !$ACC data copyin(PHIO(1:NX,1:NY)) copyout(PHIE(1:NX,1:NY))
    !$ACC kernels
    !$ACC loop 
    do iy = 1, NY
    do ix = 1, NX
      PHIE(ix,iy) = PHIO(ix,iy)
    end do 
    end do
    !$ACC end kernels
    !$ACC end data
#if defined(USE_NVTX)
    call nvtxEndRange()
#endif

    if ( tol*nrmbsq >= nrmsq ) then
      lconv = .true.
      exit
    endif 
  end do
#if defined(USE_NVTX)
  call nvtxEndRange()
#endif

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
#if 0
  do iy = 1, NY
  do ix = 1, NX
    write (99, '(2I10,1F13.4)') ix,iy,PHIE(ix,iy)
  end do
    write (99, *) 
  end do
#endif
  write (99, '(2I10,1F13.4)') NX/2,NY/2,PHIE(ix,iy)


  elp(3) = omp_get_wtime() - elp0

  write(6, '("init     : ",1F13.4,"sec.")') elp(1)
  write(6, '("main loop: ",1F13.4,"sec.")') elp(2)
  write(6, '("i/o      : ",1F13.4,"sec.")') elp(3)

  deallocate( PHIE )
  deallocate( PHIO )
  deallocate( RHO  )

  stop
end program main
