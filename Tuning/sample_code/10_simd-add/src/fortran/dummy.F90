! Copyright 2024 Research Organization for Information Science and Technology
subroutine dummy (ns, va, vb, s)
  use mytype,only: DP
  implicit none
  integer,intent(in)          :: ns
  real(kind=DP),intent(in)    :: va(1:ns)
  real(kind=DP),intent(in)    :: vb(1:ns)
  integer,intent(out)         :: s
  s = 0
end subroutine dummy
