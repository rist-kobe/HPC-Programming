! Copyright 2024 Research Organization for Information Science and Technology
!!$---------------------------------------------------------------------
!!$ calc_iniposit.f90
!!$ Author: Yukihiro Ota (yota@rist.or.jp)
!!$ Last update: 1 Nov 2016
!!$---------------------------------------------------------------------

subroutine calc_iniposit(icon,NRD)

  use common_par,only: num_partot,unit_std_out,leng
  
  use common_var,only: rx0,ry0,rz0

  implicit none
  integer,intent(inout) :: icon
  integer,intent(in) :: NRD

  !! /* local variables */
  integer :: ii,ij
  integer :: ix,iy,iz
  integer :: iok,ird,ioffset,icount
  integer,parameter :: MAXCOUNT=100

  double precision :: crx0,cry0,crz0
  double precision :: rxij,ryij,rzij,rsqij
  double precision :: rd(1:NRD)

  !! generate a random sequence
  call get_rnd_array(NRD,rd)
  
  !! get position of 1st atom 
  rx0(1) = leng*( rd(1)-0.5d0 )
  ry0(1) = leng*( rd(2)-0.5d0 )
  rz0(1) = leng*( rd(3)-0.5d0 )

  !! generate position of other atoms
  iok     = 0
  ii      = 2
  ird     = 3
  icount  = 1
  ioffset = 2
  loopii:do while ( ii .le. num_partot )

     ix = 3*(ioffset-1) + 1
     iy = ix + 1
     iz = ix + 2
     ird = ird + 3
     
     crx0 = leng*( rd(ix)-0.5d0 )
     cry0 = leng*( rd(iy)-0.5d0 )
     crz0 = leng*( rd(iz)-0.5d0 )

     !! check inter-atom distance
     do ij = 1, ii-1

        rxij = crx0 - rx0(ij)
        ryij = cry0 - ry0(ij)
        rzij = crz0 - rz0(ij)

        !! periodic boundary condition
        rxij = rxij - dnint( rxij/leng )*leng
        ryij = ryij - dnint( ryij/leng )*leng
        rzij = rzij - dnint( rzij/leng )*leng
        
        !! distance criterion: removing too short
        !! inter-atom distance
        rsqij = rxij*rxij + ryij*ryij + rzij*rzij
        if ( rsqij .lt. 1.0d0 ) then
           ioffset = ioffset + 1
           iok = 1
           exit
        end if
        
     end do

     !! update if iok=0 (and otherwise ... )
     if ( iok .eq. 0 ) then
        rx0(ii) = crx0
        ry0(ii) = cry0
        rz0(ii) = crz0
        ii = ii + 1
     else
        !! if no remaining random variable
        if ( ird .ge. NRD-3 ) then
           call get_rnd_array(NRD,rd)
           ioffset = 1
           ird = 0
           icount = icount + 1
        end if
        iok = 0
     end if

     !! check MAXCOUNT
     if ( icount .eq. MAXCOUNT ) then
        icon = -1 
        exit
     end if
     
  end do loopii

  !! Finalization
  if ( icon .lt. 0 ) then
     write(unit_std_out,'(A)') &
          "Initialization of particle position is failed."
     write(unit_std_out,'(A)') &
          "This error may often consider a too-dense case."
     write(unit_std_out,'(A)') &
          "You carefully tune total number of particles and "
     write(unit_std_out,'(A)') &
          "unit-cell size."
     return
  end if
  
  icon = 0
  return
end subroutine calc_iniposit
