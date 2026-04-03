! Copyright 2024 Research Organization for Information Science and Technology
module mytype
  integer,parameter :: SP = kind(1.0)
  integer,parameter :: DP = selected_real_kind(2*precision(1.0_SP))
  integer,parameter :: NSIZE = 32000
  integer,parameter :: NREP = 100000
  type tcmp 
    real(kind=DP) :: a
    real(kind=DP) :: b
  end type tcmp 
  type tcmpv
    real(kind=DP) :: a(1:NSIZE)
    real(kind=DP) :: b(1:NSIZE)
  end type tcmpv
end module mytype
