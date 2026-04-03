! Copyright 2024 Research Organization for Information Science and Technology
!!$---------------------------------------------------------------------
!!$ calc_force.f90
!!$ Author: Yukihiro Ota (yota@rist.or.jp)
!!$ Last update: 1 Nov 2016
!!$---------------------------------------------------------------------

!!$---Subroutine: calc_force
subroutine calc_force (NTOT,rtx,rty,rtz)

  use common_par,only: leng,LJ24,RC,RCSQ
  
  use common_var,only: fx,fy,fz
  
  implicit none
  integer,intent(in)::NTOT
  double precision ,intent(in)::rtx(1:NTOT),rty(1:NTOT),rtz(1:NTOT)

  !! /* local variables */
  integer :: ii, ij

  double precision :: rtxi,rtyi,rtzi
  double precision :: rtxij,rtyij,rtzij
  double precision :: rtsqij
  double precision :: srt2,srt6,srt12
  double precision :: fxi,fyi,fzi
  double precision :: fxij,fyij,fzij

  !! clear
  do ii = 1, NTOT
     fx(ii) = 0.0d0
     fy(ii) = 0.0d0
     fz(ii) = 0.0d0
  end do
    
  !! main loop
  loopii:do ii = 1, NTOT-1

     rtxi=rtx(ii)
     rtyi=rty(ii)
     rtzi=rtz(ii)
     fxi=fx(ii)
     fyi=fy(ii)
     fzi=fz(ii)
     
     loopij:do ij = ii+1, NTOT

        rtxij = rtxi - rtx(ij)
        rtyij = rtyi - rty(ij)
        rtzij = rtzi - rtz(ij)
        
        !! periodic boundary condition
        rtxij = rtxij - dnint( rtxij/leng )*leng
        rtyij = rtyij - dnint( rtyij/leng )*leng
        rtzij = rtzij - dnint( rtzij/leng )*leng

        !! sudden cutoff in potential:
        !! no force if (inter-atom distance) > RC
        if ( abs(rtxij) .gt. RC ) cycle
        if ( abs(rtyij) .gt. RC ) cycle
        if ( abs(rtzij) .gt. RC ) cycle

        rtsqij = rtxij*rtxij + rtyij*rtyij + rtzij*rtzij
        if ( rtsqij .gt. RCSQ ) cycle        

        !! calculate force
        srt2  = 1.0d0/rtsqij
        srt6  = srt2**3
        srt12 = srt6**2
        srt2  = LJ24*(2.0d0*srt12 - srt6)/rtsqij

        fxij = srt2*rtxij
        fyij = srt2*rtyij
        fzij = srt2*rtzij

        fxi = fxi + fxij
        fyi = fyi + fyij
        fzi = fzi + fzij

        !! action-reaction law
        fx(ij) = fx(ij) - fxij
        fy(ij) = fy(ij) - fyij
        fz(ij) = fz(ij) - fzij
                 
     end do loopij

     !! update
     fx(ii) = fxi
     fy(ii) = fyi
     fz(ii) = fzi
     
  end do loopii
  
  return
end subroutine calc_force
!!$---End of Subroutine: calc_force


!!$---Subroutine: calc_force_diffuse (no pbc)
subroutine calc_force_diffuse (NTOT,rtx,rty,rtz)

  use common_par,only: LJ24,RC,RCSQ
  
  use common_var,only: fx,fy,fz
  
  implicit none
  integer,intent(in)::NTOT
  double precision,intent(in)::rtx(1:NTOT),rty(1:NTOT),rtz(1:NTOT)

  !! /* local variables */
  integer :: ii, ij

  double precision :: rtxi,rtyi,rtzi
  double precision :: rtxij,rtyij,rtzij
  double precision :: rtsqij
  double precision :: srt2,srt6,srt12
  double precision :: fxi,fyi,fzi
  double precision :: fxij,fyij,fzij

  !! clear
  do ii = 1, NTOT
     fx(ii) = 0.0d0
     fy(ii) = 0.0d0
     fz(ii) = 0.0d0
  end do
    
  !! main loop
  loopii:do ii = 1, NTOT-1

     rtxi=rtx(ii)
     rtyi=rty(ii)
     rtzi=rtz(ii)
     fxi=fx(ii)
     fyi=fy(ii)
     fzi=fz(ii)
     
     loopij:do ij = ii+1, NTOT

        rtxij = rtxi - rtx(ij)
        rtyij = rtyi - rty(ij)
        rtzij = rtzi - rtz(ij)
        
        !! sudden cutoff in potential:
        !! no force if (inter-atom distance) > RC
        if ( abs(rtxij) .gt. RC ) cycle
        if ( abs(rtyij) .gt. RC ) cycle
        if ( abs(rtzij) .gt. RC ) cycle

        rtsqij = rtxij*rtxij + rtyij*rtyij + rtzij*rtzij
        if ( rtsqij .gt. RCSQ ) cycle 

        !! calculate force
        srt2  = 1.0d0/rtsqij
        srt6  = srt2**3
        srt12 = srt6**2
        srt2  = LJ24*(2.0d0*srt12 - srt6)/rtsqij

        fxij = srt2*rtxij
        fyij = srt2*rtyij
        fzij = srt2*rtzij

        fxi = fxi + fxij
        fyi = fyi + fyij
        fzi = fzi + fzij

        !! action-reaction law
        fx(ij) = fx(ij) - fxij
        fy(ij) = fy(ij) - fyij
        fz(ij) = fz(ij) - fzij
                 
     end do loopij

     !! update
     fx(ii) = fxi
     fy(ii) = fyi
     fz(ii) = fzi
     
  end do loopii
  
  return
end subroutine calc_force_diffuse
!!$---End of Subroutine: calc_force_diffuse
