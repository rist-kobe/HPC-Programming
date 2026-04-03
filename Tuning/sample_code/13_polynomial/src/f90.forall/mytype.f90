! Copyright 2023 Research Organization for Information Science and Technology
module mytype
  implicit none
  integer,parameter :: SP = kind(1.0) 
  integer,parameter :: pd = 2*precision(1.0_SP)
  integer,parameter :: DP = selected_real_kind(pd)
end module mytype
