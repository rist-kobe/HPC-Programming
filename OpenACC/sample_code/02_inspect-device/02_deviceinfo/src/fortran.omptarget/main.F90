! Copyright 2025 Research Organization for Information Science and Technology
! Reference
! * Tom Deakin and Timothy G. Mattson, 
!   "Programming your GPU with OpenMP: Performance portability for GPUs"
!   (MIT, 2023) Chap.8 and Appendix.
program main
  use omp_lib
  implicit none
  
  integer :: ng, dev, host_id

  write(6,'("openmp_version=",1I10)') openmp_version   
  ng = omp_get_num_devices() 

  if ( ng .eq. 0 ) then
     write(6,'("There is no available device.")')
     stop
  else  
     write(6,'(1I03,"  OpenMP Target Capable devices are detected.")') ng
  end if

  host_id = omp_get_initial_device()

  write(6,'("Host ID               :",1I03)') host_id

  !$OMP parallel
  host_id = omp_get_initial_device()
  write(6,'("Host ID (in omp parallel) :",1I03)') host_id
  !$OMP end parallel


  dev = 0
  call omp_set_default_device(dev)

  write(6,'("Device ID             :",1I03)') dev

  ! I think there is no API to query hardware information 
  ! of deveices in OpenMP. :( 
  stop
end program main
