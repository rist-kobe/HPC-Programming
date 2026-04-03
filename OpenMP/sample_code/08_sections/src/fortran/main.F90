! Copyright 2024 Research Organization for Information Science and Technology
program main
  implicit none 
!$omp parallel
  !$omp sections
    !$omp section 
    call subA()
    !$omp section 
    call subB()
    !$omp section 
    call subC()
    !$omp section 
    call subD()
  !$omp end sections
!$omp end parallel   
  stop
end program main

subroutine subA()
  use omp_lib
  implicit none 
  integer ith
  ith = omp_get_thread_num()
  write(6,'("Hello. I am subA, with thread id ",1I3)') ith
end subroutine subA

subroutine subB()
  use omp_lib
  implicit none 
  integer ith;
  ith = omp_get_thread_num()
  write(6,'("Hello. I am subB, with thread id ",1I3)') ith
end subroutine subB

subroutine subC()
  use omp_lib
  implicit none 
  integer ith;
  ith = omp_get_thread_num()
  write(6,'("Hello. I am subC, with thread id ",1I3)') ith
end subroutine subC

subroutine subD()
  use omp_lib
  implicit none 
  integer ith;
  ith = omp_get_thread_num()
  write(6,'("Hello. I am subD, with thread id ",1I3)') ith
end subroutine subD
