! Copyright 2024 Research Organization for Information Science and Technology
!!$---------------------------------------------------------------------
!!$ rndvar.f90
!!$ Author: Yukihiro Ota (yota@rist.or.jp)
!!$ Last update: 1 Nov 2016
!!$
!!$---------------------------------------------------------------------

!!$--- Subroutine: set_rndseed
!!$  Initialization of pseudorandom variabels generated 
!!$  by the MT method
!!$
!!$   See 
!!$        www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/mt.html
!!$        www001.upp.so-net.ne.jp/isaku/rand.html
!!$
!!$    Any nonzero 32-bit integer can be used as a seed
!!$
!!$    After calling this subroutine, grand() generates
!!$    one pseudorandom real number (double), uniformly
!!$    distributed on [0,1] interval
subroutine set_rndseed(icon)

  use mtmod,only:sgrnd
  
  implicit none
  integer,intent(inout)::icon

  !! /* local variables */
  integer :: icount
  
  
  if ( icon .gt. 1 ) then
     !!  .... to make seed by system clock ....
     call system_clock (icount)
  else        
     !! debug mode
     !! 4357 is a default value
     icount = 4357
  end if
  call sgrnd(icount)

  icon = 0
  return
end subroutine set_rndseed
!!$--- End of Subroutine: set_rndseed


!!$--- Subroutine: get_rnd_array
subroutine get_rnd_array(NRD,rd)

  use mtmod,only:grnd

  implicit none
  integer,intent(in) :: NRD
  double precision,intent(out) :: rd(1:NRD)

  !! /* local variables */
  integer :: ird

  do ird = 1, NRD
     rd(ird) = grnd()
  end do

  return
end subroutine get_rnd_array
!!$--- End of Subroutine: get_rnd_array


!!$--- Subroutine: get_rnd_array_for_BM
subroutine get_rnd_array_for_BM(NRD,rdx,rdy)

  use mtmod,only:grnd

  implicit none
  integer,intent(in) :: NRD
  double precision,intent(out) :: rdx(1:NRD)
  double precision,intent(out) :: rdy(1:NRD)

  !! /* local variables */
  integer :: iset
  integer :: ird

  double precision :: vv

  do ird = 1, NRD
     
     !! generate two uniform random variables in [0,1]
     iset = 0
     do while ( iset .eq. 0 )
        vv = grnd()
        !! to avoid problematic behavior in BM transformation
        if ( vv .gt. 1.d-6 ) then
           rdx(ird) = vv
           iset = 1
        end if
     end do
     rdy(ird) = grnd()

  end do

  return
end subroutine get_rnd_array_for_BM
!!$--- End of Subroutine: get_rnd_array_for_BM


!!$--- Subroutine: boxmueller
!!$ Box-Mueller transformation:
!!$
!!$ D. U. Lee, J. D. Villasenor, W. Luk, and P. H. W. Leong,
!!$ A hardware Gaussian noise generator using Box-Mueller method
!!$ and its error analysis
!!$ IEEE Transactions on Computers, Vol.55-6 pp.659-671 (2006).
!!$
!!$ This code is made, according to a Japanese textbook written by 
!!$ Tetsuaki Yotsuji (2010, www.pleiades-publishing.co.jp). 
!!$    
!!$ This is the most primitive (and simplest) generator of Gaussian 
!!$ white noise from two uniform randam variables
subroutine boxmueller(icon,rdx,rdy,sigma,eta)

  use common_par,only: dpi
  
  implicit none
  integer,intent(inout) :: icon
  double precision,intent(in) :: rdx,rdy,sigma
  double precision,intent(out):: eta

  !! /* local variables */
  integer,save :: iset
  integer :: itmp1, itmp2

  double precision :: wx,wy
  
  !! /* initialization of iset */
  if ( icon .eq. 1 ) then
     iset = 1
     icon = 0
     return
  end if
  
  !!  /* Box-Mueller transformation */
  !! x and y are two uniform random variables in [0,1]
  wx = sqrt(-2.0d0*log(rdx))*cos(rdy*dpi)
  wy = sqrt(-2.0d0*log(rdx))*sin(rdy*dpi)


  !! /* Gaussian random variable with variant sigma^2 */
  !! wx and wy are taken with an alternative manner
  eta = dble(iset)*wx + dble(iset-1)*wy
  eta = sigma*eta


  !! /* for the next calculation */
  itmp1 = iset+1
  itmp2 = itmp1/2
  iset = itmp1 - itmp2*2

  return
end subroutine boxmueller
!!$--- End of Subroutine: boxmueller
