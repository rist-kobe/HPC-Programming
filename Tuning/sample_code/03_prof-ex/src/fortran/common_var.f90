! Copyright 2024 Research Organization for Information Science and Technology
!!$---------------------------------------------------------------------
!!$ common_var.f90
!!$ Author: Yukihiro Ota (yota@rist.or.jp)
!!$ Last update: 1 Nov 2016
!!$
!!$ set common variables
!!$---------------------------------------------------------------------

module common_var

  use common_par,only:num_partot
  
  implicit none

  !!--------------------------------------------------------------------
  !! /* variables */
  !!--------------------------------------------------------------------
  ! position (dimensionless)
  double precision,allocatable :: rx(:), ry(:), rz(:)
  double precision,allocatable :: rx0(:), ry0(:), rz0(:)

  ! mass (atomic mass unit)
  double precision,allocatable :: mass(:)

  ! force (dimensionless)
  double precision,allocatable :: fx(:), fy(:), fz(:)

contains

  subroutine get_memory_of_array

    implicit none

    allocate ( rx(1:num_partot) )
    allocate ( ry(1:num_partot) )
    allocate ( rz(1:num_partot) )
    allocate ( rx0(1:num_partot) )
    allocate ( ry0(1:num_partot) )
    allocate ( rz0(1:num_partot) )
    allocate ( mass(1:num_partot) )
    allocate ( fx(1:num_partot) )
    allocate ( fy(1:num_partot) )
    allocate ( fz(1:num_partot) )

    return
  end subroutine get_memory_of_array

  subroutine del_memory_of_array

    implicit none

    deallocate ( rx )
    deallocate ( ry )
    deallocate ( rz )
    deallocate ( rx0 )
    deallocate ( ry0 )
    deallocate ( rz0 )
    deallocate ( mass )
    deallocate ( fx )
    deallocate ( fy )
    deallocate ( fz )

    return
  end subroutine del_memory_of_array

end module common_var
