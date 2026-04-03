! Copyright 2024 Research Organization for Information Science and Technology
!!$---------------------------------------------------------------------
!!$ calc_inivel.f90
!!$ Author: Yukihiro Ota (yota@rist.or.jp)
!!$ Last update: 1 Nov 2016
!!$---------------------------------------------------------------------

subroutine calc_inivel(icon,NRD,NTOT)

  use common_par,only: tempt, temptunit

  use common_var,only: rx,ry,rz,mass
  
  implicit none
  integer,intent(inout) :: icon
  integer,intent(in) :: NRD
  integer,intent(in) :: NTOT

  !! /* local variables */
  integer :: ii,ird
  
  double precision :: tm, eta
  double precision :: ru, rv
  double precision :: pxtot,pytot,pztot
  double precision :: sigma(1:NTOT)
  double precision :: rdx(1:NRD),rdy(1:NRD)

  !! dimensionless temperature
  tm = tempt/temptunit

  !! variance of the Maxwell distribution with temperature, tm
  do ii =1, NTOT
     sigma(ii) = sqrt(tm/mass(ii))
  end do

  !! generate random sequence for BM transformation
  call get_rnd_array_for_BM(NRD,rdx,rdy)
  
  !! initialize box-mueller transformation routine
  icon = 1
  ru   = 0.0d0
  rv   = 0.0d0
  call boxmueller(icon,ru,rv,tm,eta)

  !! generate velocity following the Maxwell distribution
  !! with temperature, tm
  do ii = 1, NTOT

     tm = sigma(ii)

     ird = 3*(ii-1)+1
     ru = rdx(ird)
     rv = rdy(ird)
     call boxmueller(icon,ru,rv,tm,eta)
     rx(ii)=eta

     ird = ird + 1
     ru = rdx(ird)
     rv = rdy(ird)
     call boxmueller(icon,ru,rv,tm,eta)
     ry(ii)=eta

     ird = ird + 1
     ru = rdx(ird)
     rv = rdy(ird)
     call boxmueller(icon,ru,rv,tm,eta)
     rz(ii)=eta
     
  end do

  !! remove excess total momentum
  pxtot=0.0d0
  pytot=0.0d0
  pztot=0.0d0
  do ii = 1, NTOT
     pxtot = pxtot + mass(ii)*rx(ii)
     pytot = pytot + mass(ii)*ry(ii)
     pztot = pztot + mass(ii)*rz(ii)
  end do
  pxtot = pxtot/NTOT
  pytot = pytot/NTOT
  pztot = pztot/NTOT
  
  do ii = 1, NTOT
     rx(ii) = rx(ii) - pxtot / mass(ii)
     ry(ii) = ry(ii) - pytot / mass(ii)
     rz(ii) = rz(ii) - pztot / mass(ii)
  end do

  return
end subroutine calc_inivel