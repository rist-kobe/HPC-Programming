! Copyright 2024 Research Organization for Information Science and Technology
!!$---------------------------------------------------------------------
!!$ calc_param.f90
!!$ Author: Yukihiro Ota (yota@rist.or.jp)
!!$ Last update: 1 Nov 2016
!!$---------------------------------------------------------------------

subroutine calc_param

  use common_par,only:num_partot,dt,tempt,epsLJ,sigLJ,len_cell,&
       NA,KB,MAMU,lunit,vunit,tunit,temptunit,ndensunit,&
       leng,ndens,dtu,dtusq,nstep_upbd,dt_upbd
  
  implicit none
    
  lunit      = sigLJ
  vunit      = sqrt(1.0d-3*epsLJ/(MAMU*NA))
  tunit      = 0.1d0*lunit/vunit
  temptunit  = epsLJ/KB
  ndensunit  = 1.d0/(lunit**3)

  leng       = len_cell/lunit
  ndens      = num_partot/(leng**3)

  dtu        = dt*1.0d-6/tunit
  nstep_upbd = (0.5d0*leng/sqrt(3.0d0*tempt/temptunit))/dtu
  dt_upbd    = 1.0d-1*tunit/sqrt(3.0d0*tempt/temptunit)
  dtusq      = dtu*dtu
  
  return
end subroutine calc_param
