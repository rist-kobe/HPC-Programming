! Copyright 2024 Research Organization for Information Science and Technology
module mytype
  integer,parameter :: SP = kind(1.0)
  integer,parameter :: DP = selected_real_kind(2*precision(1.0_SP))
  integer,parameter :: NSIZE = 80000
  integer,parameter :: NREP = 10000
end module mytype
