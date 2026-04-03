! Copyright 2025 Research Organization for Information Science and Technology
program main
  use openacc
  implicit none
  
  integer(kind=acc_device_kind) :: dev_type
  integer :: ng, dev
  integer(kind=8) :: dev_mem

  character(len=128) ::cbuf 

  write(6,'("openacc_version=",1I10)') openacc_version
  dev_type = acc_get_device_type()
  ng = acc_get_num_devices(dev_type) 

  if ( ng .eq. 0 ) then
     write(6,'("There is no available device.")')
     stop
  else  
     write(6,'(1I03,"  OpenACC Capable devices are detected.")') ng
  end if

  dev = 0

  call acc_set_device_num(dev, dev_type)

  write(6,'("Device ID             :",1I03)') dev

  dev_mem = acc_get_property(dev, dev_type, 1) ! acc_property_memory

  call acc_get_property_string(dev, dev_type, 3, cbuf) ! acc_property_name
  write (6, '("Device ",1I03,": ",1A)') dev, cbuf

  call acc_get_property_string(dev, dev_type, 4, cbuf) ! acc_property_vendor
  write (6, '("  Vendor : ",1A)') cbuf

  call acc_get_property_string(dev, dev_type, 5, cbuf) ! acc_property_driver
  write (6, '("  Driver : ",1A)') cbuf

  write (6, '("  Total amount of device memory:   ",1F8.2," Mibytes")') &
&   real(dev_mem) / 1048576.0

  stop
end program main
