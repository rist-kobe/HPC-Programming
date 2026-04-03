! Copyright 2024 Research Organization for Information Science and Technology
!!$---------------------------------------------------------------------
!!$ write_output_pos.f90
!!$ Author: Yukihiro Ota (yota@rist.or.jp)
!!$ Last update: 1 Nov 2016
!!$---------------------------------------------------------------------

!!$---Subroutine: write_output_pos
subroutine write_output_pos(iunit,NTOT,rtx,rty,rtz)

  use common_par,only: num_parA,num_parB

  implicit none
  integer,intent(in) :: iunit
  integer,intent(in) :: NTOT
  double precision,intent(in) :: rtx(1:NTOT),rty(1:NTOT),rtz(1:NTOT)

  !! /* local variables */
  integer :: ii

  write(iunit,'(1I8)') NTOT
  write(iunit,'(A)') 
  do ii = 1, num_parA
     write(iunit,'(1A5,3E23.8)') 'H', &
          rtx(ii),rty(ii),rtz(ii)
  end do
  do ii = num_parA+1, num_parA+num_parB
     write(iunit,'(1A5,3E23.8)') 'O', &
          rtx(ii),rty(ii),rtz(ii)
  end do

  return
end subroutine write_output_pos
!!$---End of Subroutine: write_output_pos


!!$---Subroutine: write_output_pos_diffuse
!!$ When you face on some problem for visualization a diffusion
!!$ process, you may use this subroutine. A too-large particle
!!$ coordinate is rewritten by a threshold value, thleng.
!!$ BUT, I recommend that you carefully tune the number of time
!!$ step, before using this subroutine. 
subroutine write_output_pos_diffuse(iunit,NTOT,rtx,rty,rtz)

  use common_par,only: num_parA,num_parB,leng

  implicit none
  integer,intent(in) :: iunit
  integer,intent(in) :: NTOT
  double precision,intent(in) :: rtx(1:NTOT),rty(1:NTOT),rtz(1:NTOT)

  !! /* local variables */
  integer :: ii

  double precision :: rxi,ryi,rzi
  double precision :: thleng

  thleng = 3.0d0*leng
  
  write(iunit,'(1I8)') NTOT
  write(iunit,'(A)') 
  do ii = 1, num_parA

     rxi = rtx(ii)
     ryi = rty(ii)
     rzi = rtz(ii)

     if ( abs(rxi) .gt. thleng ) rxi=thleng*abs(rxi)/rxi
     if ( abs(ryi) .gt. thleng ) ryi=thleng*abs(ryi)/ryi
     if ( abs(rzi) .gt. thleng ) rzi=thleng*abs(rzi)/rzi
     
     write(iunit,'(1A5,3E23.8)') 'H', &
          rxi,ryi,rzi
  end do
  do ii = num_parA+1, num_parA+num_parB
     rxi = rtx(ii)
     ryi = rty(ii)
     rzi = rtz(ii)

     if ( abs(rxi) .gt. thleng ) rxi=thleng*abs(rxi)/rxi
     if ( abs(ryi) .gt. thleng ) ryi=thleng*abs(ryi)/ryi
     if ( abs(rzi) .gt. thleng ) rzi=thleng*abs(rzi)/rzi
     
     write(iunit,'(1A5,3E23.8)') 'O', &
          rxi,ryi,rzi
  end do

  return
end subroutine write_output_pos_diffuse
!!$---End of Subroutine: write_output_pos_diffuse
