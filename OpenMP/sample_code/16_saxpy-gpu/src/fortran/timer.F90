! Copyright 2024 Research Organization for Information Science and Technology
module timer
  use mytype, only: DP
  implicit none
  integer,private :: ielp1, icount_rate, icount_max
contains
  subroutine get_elp_time(ioffset, tv)
     integer,intent(inout) :: ioffset
     real(kind=DP),intent(out),optional :: tv
     if ( .not. present(tv) ) then
        call system_clock(ielp1, icount_rate, icount_max)
        ioffset = ielp1 
        return
     else
        call system_clock(ielp1, icount_rate, icount_max)
        if ( ielp1 .ge. ioffset ) then
           tv = (ielp1 - ioffset) / dble(icount_rate)
        else
           tv = (ielp1 + icount_max + 1 - ioffset) / dble(icount_rate)
        end if
        return
     end if
  end subroutine get_elp_time
  !
  real(kind=DP) function get_cpu_time()
     call cpu_time( get_cpu_time )
  end function get_cpu_time
end module
