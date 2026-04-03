! Copyright 2023 Research Organization for Information Science and Technology
module mytype
  integer,parameter :: SP = kind(1.0)
  integer,parameter :: DP = selected_real_kind(2*precision(1.0_SP))
end module mytype
