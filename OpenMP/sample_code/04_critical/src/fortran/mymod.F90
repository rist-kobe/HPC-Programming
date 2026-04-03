! Copyright 2024 Research Organization for Information Science and Technology
module mymod
  implicit none
  integer,parameter :: SP = kind(1.0)
  integer,parameter :: DP = selected_real_kind(2*precision(1.0_SP))
  integer,private :: u(2)

contains
  subroutine decomp_size (ns, tid, nt, ifrom, ito)
     integer,intent(in) :: ns, tid, nt
     integer,intent(out) :: ifrom, ito
     integer :: delta
     delta = 1 + ns / nt
     ifrom = 1 + tid * delta; ito   = MIN(ifrom+delta-1, ns)
  end subroutine
 
  ! Indeed, the working array, u, is **NOT** needed. 
  ! This implementation is nothing less than an example.
  function func (a, ifrom, ito) 
    integer :: func
    integer,contiguous,intent(in) :: a(:)
    integer,intent(in) :: ifrom
    integer,intent(in) :: ito

    integer :: i, ifrom1, ifrom2

    if ( mod(ifrom,2) .eq. 1 ) then
       ifrom1 = ifrom
       ifrom2 = ifrom + 1
    else
       ifrom1 = ifrom + 1
       ifrom2 = ifrom
    end if

    u(1) = 0;
    do i = ifrom1, ito, 2
       u(1) = u(1) + a(i)
    end do

    u(2) = 0;
    do i = ifrom2, ito, 2
       u(2) = u(2) - a(i)
    end do

    func = u(1) + u(2)
  end function func
end module mymod
