! Copyright 2024 Research Organization for Information Science and Technology
program main
  use omp_lib
  implicit none
  integer :: tid0,tid1,tid2,tid3 

  !$omp parallel private(tid0)
    !$omp single
      tid0 = omp_get_thread_num()
      write(6,'("I am ",1I0,". Hello world from a random thread.")') tid0 
      !$omp task private(tid1)
      tid1 = omp_get_thread_num()
      write(6,'("task 1 run by tid=",1I0,", queued by ",1I0)') tid1,tid0
      !$omp end task 
      !$omp task private(tid2)
      tid2 = omp_get_thread_num()
      write(6,'("task 2 run by tid=",1I0,", queued by ",1I0)') tid2,tid0
      !$omp end task 
      !$omp task private(tid3)
      tid3 = omp_get_thread_num()
      write(6,'("task 3 run by tid=",1I0,", queued by ",1I0)') tid3,tid0
      !$omp end task 
    !$omp end single
  !$omp end parallel
  stop
end program main
