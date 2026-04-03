! Copyright 2024 Research Organization for Information Science and Technology 
program main
!$  use omp_lib

  integer :: nt, tid

!$  write(6,'("openmp_version=",1I10)') openmp_version

!$OMP PARALLEL private(nt, tid)

!$  nt  = omp_get_num_threads()
!$  tid = omp_get_thread_num()

  if ( tid .eq. 0 ) then
     write(6,'("I am the primary (0) thread.&
& Total number of threads is ",1I5)') nt
  end if
!$OMP END PARALLEL

end program main
