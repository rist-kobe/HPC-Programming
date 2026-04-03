! Copyright 2025 Research Organization for Information Science and Technology
! Reference
! * OpenACC Getting started guid, 2.6 and 2.9;
!   https://docs.nvidia.com/hpc-sdk/compilers/openacc-gs/index.html
module mymod
  implicit none
  integer,parameter :: SP = kind(1.0)
  integer,parameter :: DP = selected_real_kind(2*precision(1.0_SP))

  type vector
     integer :: nsize
     real(kind=DP),allocatable,dimension(:) :: x, y, z
  end type

  type(vector) :: s
  !$ACC declare copyin(s)
end module mymod
