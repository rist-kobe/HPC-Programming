! Copyright 2024 Research Organization for Information Science and Technology
program main
  implicit none
!$omp parallel
  ! Each section is executed exactly once by one thread of the team.
  ! There is no ordering guarantee among the sections, and an implied
  ! barrier exists at "end sections" (unless a nowait clause is added).
  !$omp sections
    !$omp section
    call greet("subA")
    !$omp section
    call greet("subB")
    !$omp section
    call greet("subC")
    !$omp section
    call greet("subD")
  !$omp end sections
!$omp end parallel

contains

  subroutine greet(name)
    use omp_lib
    character(len=*), intent(in) :: name
    integer :: ith
    ith = omp_get_thread_num()
    write(6,'("Hello. I am ",A,", with thread id ",1I3)') name, ith
  end subroutine greet

end program main
