! Copyright 2024 Research Organization for Information Science and Technology
!!$---------------------------------------------------------------------
!!$ calc_mass.f90
!!$ Author: Yukihiro Ota (yota@rist.or.jp)
!!$ Last update: 1 Nov 2016
!!$---------------------------------------------------------------------

subroutine calc_mass

  use common_par,only:num_parA,num_parB,mamu_parA,mamu_parB

  use common_var,only:mass
  
  implicit none

  !! /* local variables */
  integer :: ii

  do ii = 1 , num_parA
     mass(ii) = mamu_parA
  end do
  do ii = num_parA+1 , num_parA+num_parB
     mass(ii) = mamu_parB
  end do
    
  return
end subroutine calc_mass
