! Copyright 2024 Research Organization for Information Science and Technology
!*************************************************************************
!  Check resolution of your timer
!  Author:      Yukihiro Ota (yota@rist.or.jp)
!  Description: The original idea comes from Wadleigh and Crawford,
!               "Software Optimization for HPC: Creating Faster 
!                Applications" (2000) pp.136-138 
!*************************************************************************

!=========================================================================
!   module: mytype 
!=========================================================================
module mytype
  implicit none
  integer,parameter :: SP = kind(1.0)
  integer,parameter :: DP = selected_real_kind(2*precision(1.0_SP))
end module mytype
!=========================================================================
!   main 
!=========================================================================
program main

  use mytype,only: DP

  implicit none

  !! local var
  integer :: i, nn, cnt1, cnt2, cnt_rate, cnt_max

  real(kind=DP) :: tval, tval0 
  real(kind=DP),parameter :: ZERO = 0.0_DP

  !! interface to function
  interface
    
    function func(nn)
      integer :: func
      integer,intent(in) :: nn
    end function func

  end interface

  write(6,'("----------------------------------------------------------")')

  !! check resolution of wallclock timer  
  tval = -1.0_DP 
  nn = 0
  do while ( tval .le. ZERO )
    nn = nn + 1
    call system_clock (cnt1)
    i = func(nn)
    call system_clock (cnt2, cnt_rate, cnt_max)
    if ( cnt2 .ge. cnt1 ) then
       tval = (cnt2 - cnt1) / dble(cnt_rate)
    else
       tval = (cnt2 - cnt1 + cnt_max + 1) / dble(cnt_rate)
    end if
  end do

  write(6,'("[Check resolution of wallclock timer]")')
  write(6,'("It took ",1I7," &
&   iterations to generate a none-zero time")') nn 
  if ( nn == 1 ) then
    write(6,'(" timer resolution less than or equal to ",1F15.9)') tval
  else
    write(6,'(" timer resolution is ",1F15.9," sec.")') tval
  end if

  write(6,'("----------------------------------------------------------")')

  !! check resolution of cpu timer  
  tval = -1.0_DP 
  nn = 0
  do while ( tval .le. ZERO )
    nn = nn + 1
    call cpu_time (tval0)
    i = func(nn)
    call cpu_time (tval)
    tval = tval - tval0
  end do

  write(6,'("[Check resolution of cpu timer]")')
  write(6,'("It took ",1I7," &
&   iterations to generate a none-zero time")') nn 
  if ( nn == 1 ) then
    write(6,'(" timer resolution less than or equal to ",1F15.9)') tval
  else
    write(6,'(" timer resolution is ",1F15.9," sec.")') tval
  end if

  write(6,'("----------------------------------------------------------")')

  stop
end program main
!==========================================================================
!   func 
!==========================================================================
function func (nn)

  implicit none

  integer :: func
  integer,intent(in) :: nn

  integer :: j

  func = 0
  do j = 1, nn
    func = func + 1
  end do
end function func
