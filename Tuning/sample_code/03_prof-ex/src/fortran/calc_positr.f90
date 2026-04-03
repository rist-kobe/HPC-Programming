! Copyright 2024 Research Organization for Information Science and Technology
!!$---------------------------------------------------------------------
!!$ calc_positr.f90
!!$ Author: Yukihiro Ota (yota@rist.or.jp)
!!$ Last update: 1 Nov 2016
!!$---------------------------------------------------------------------

!!$---Subroutine: calc_positr1
subroutine calc_positr1(NTOT,rtx,rty,rtz,vtx,vty,vtz)

  use common_par,only: dtu,dtusq, leng

  use common_var,only: fx,fy,fz,mass
  
  implicit none
  integer,intent(in) :: NTOT
  double precision,intent(in)   :: rtx(1:NTOT),rty(1:NTOT),rtz(1:NTOT)
  double precision,intent(inout):: vtx(1:NTOT),vty(1:NTOT),vtz(1:NTOT)

  !! /* local variables */
  integer :: ii
  
  !! update by a simple Verlet algorithm
  do ii = 1, NTOT
     vtx(ii) = rtx(ii) + dtu*vtx(ii) + 0.5d0*dtusq*fx(ii)/mass(ii)
     vty(ii) = rty(ii) + dtu*vty(ii) + 0.5d0*dtusq*fy(ii)/mass(ii)
     vtz(ii) = rtz(ii) + dtu*vtz(ii) + 0.5d0*dtusq*fz(ii)/mass(ii)
  end do

  !! periodic boundary condition
  do ii = 1, NTOT
     vtx(ii) = vtx(ii) - dnint( vtx(ii)/leng )*leng
     vty(ii) = vty(ii) - dnint( vty(ii)/leng )*leng
     vtz(ii) = vtz(ii) - dnint( vtz(ii)/leng )*leng
  end do
  
  return
end subroutine calc_positr1
!!$---End of Subroutine: calc_positr1


!!$---Subroutine: calc_positr1_diffuse
subroutine calc_positr1_diffuse(NTOT,rtx,rty,rtz,vtx,vty,vtz)

  use common_par,only: dtu,dtusq

  use common_var,only: fx,fy,fz,mass
  
  implicit none
  integer,intent(in) :: NTOT
  double precision,intent(in)   :: rtx(1:NTOT),rty(1:NTOT),rtz(1:NTOT)
  double precision,intent(inout):: vtx(1:NTOT),vty(1:NTOT),vtz(1:NTOT)

  !! /* local variables */
  integer :: ii
  
  !! update by a simple Verlet algorithm
  do ii = 1, NTOT
     vtx(ii) = rtx(ii) + dtu*vtx(ii) + 0.5d0*dtusq*fx(ii)/mass(ii)
     vty(ii) = rty(ii) + dtu*vty(ii) + 0.5d0*dtusq*fy(ii)/mass(ii)
     vtz(ii) = rtz(ii) + dtu*vtz(ii) + 0.5d0*dtusq*fz(ii)/mass(ii)
  end do
    
  return
end subroutine calc_positr1_diffuse
!!$---End of Subroutine: calc_positr1_diffuse


!!$---Subroutine: calc_positr
subroutine calc_positr (NTOT,rbx,rby,rbz,rtx,rty,rtz)

  use common_par,only: dtusq,leng

  use common_var,only: fx,fy,fz,mass
  
  implicit none
  integer,intent(in) :: NTOT
  double precision,intent(inout):: rbx(1:NTOT),rby(1:NTOT),rbz(1:NTOT)
  double precision,intent(inout):: rtx(1:NTOT),rty(1:NTOT),rtz(1:NTOT)

  !! /* local variables */
  integer :: ii

  double precision :: rxi,ryi,rzi
  
  !! perform a simple Verlet algorithm
  do ii = 1, NTOT

     rxi = 2.0d0*rtx(ii) - rbx(ii) + dtusq*fx(ii)/mass(ii)
     ryi = 2.0d0*rty(ii) - rby(ii) + dtusq*fy(ii)/mass(ii)
     rzi = 2.0d0*rtz(ii) - rbz(ii) + dtusq*fz(ii)/mass(ii)

     rbx(ii) = rtx(ii)
     rby(ii) = rty(ii)
     rbz(ii) = rtz(ii)

     !! periodic boundary condition
     rtx(ii) = rxi - dnint( rxi/leng )*leng
     rty(ii) = ryi - dnint( ryi/leng )*leng
     rtz(ii) = rzi - dnint( rzi/leng )*leng

  end do

  return
end subroutine calc_positr
!!$---End of Subroutine: calc_positr


!!$---Subroutine: calc_positr_diffuse
subroutine calc_positr_diffuse (NTOT,rbx,rby,rbz,rtx,rty,rtz)

  use common_par,only: dtusq

  use common_var,only: fx,fy,fz,mass
  
  implicit none
  integer,intent(in) :: NTOT
  double precision,intent(inout):: rbx(1:NTOT),rby(1:NTOT),rbz(1:NTOT)
  double precision,intent(inout):: rtx(1:NTOT),rty(1:NTOT),rtz(1:NTOT)

  !! /* local variables */
  integer :: ii

  double precision :: rxi,ryi,rzi
  
  !! perform a simple Verlet algorithm
  do ii = 1, NTOT

     rxi = 2.0d0*rtx(ii) - rbx(ii) + dtusq*fx(ii)/mass(ii)
     ryi = 2.0d0*rty(ii) - rby(ii) + dtusq*fy(ii)/mass(ii)
     rzi = 2.0d0*rtz(ii) - rbz(ii) + dtusq*fz(ii)/mass(ii)

     rbx(ii) = rtx(ii)
     rby(ii) = rty(ii)
     rbz(ii) = rtz(ii)

     rtx(ii) = rxi
     rty(ii) = ryi
     rtz(ii) = rzi

  end do

  return
end subroutine calc_positr_diffuse
!!$---End of Subroutine: calc_positr_diffuse