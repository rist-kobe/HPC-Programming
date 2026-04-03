! Copyright 2024 Research Organization for Information Science and Technology
module mytype
  implicit none
  integer,parameter :: SP=kind(1.0)
  integer,parameter :: DP=selected_real_kind(2*SP)
end module mytype
