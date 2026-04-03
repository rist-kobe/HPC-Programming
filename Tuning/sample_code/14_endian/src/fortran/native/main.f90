! Copyright 2024 Research Organization for Information Science and Technology
!---------------------------------------------------------------------72
! Check endianness
! Author:      Yukihiro Ota (yota@rist.or.jp)
! Last update: June 1 2018
!
! Description:
! Expected output of 0x12345678 
! 1. Big Endian
! Byte-ID:  0  1  2  3
!          12 34 56 78 
! 2. Little Endian
! Byte-ID:  0  1  2  3
!          78 56 34 12 
!
! Remark 1: Endianness only affects byte order. 
! Since char is exactly 1 BYTE, it is the same as
! all endianness.
!
! Remark 2: You can check everything, via hexdump or xxd.
! (ex) hexdump -C -v output
!
! Remark 3: In typical Fortran compiler, endianness can be changed
! in several ways.
! (ex) In compiling
!      gfortran -fconvert=swap foo.f90 -o foo.exe
! (ex) Use Environmental Variables
!      This depends on kinds of architecture 
!---------------------------------------------------------------------72
module mytype
  implicit none 
  integer,parameter :: SP = kind(1.0)
  integer,parameter :: pd = 2*precision(1.0_SP)
  integer,parameter :: DP = selected_real_kind(pd)
end module mytype

program main
  use mytype, only: DP

  implicit none

  character(len=28) :: cbuff
  integer :: ibuff, ibsgn
  real(kind=DP) :: rbuff

  cbuff = 'Is your num big or little?'
  ibuff = X'12345678' !! 305419896
  ibsgn = -305419896  !! 0xedcba988 
  rbuff = 1.4_DP

  write(6,'(1a)') "[Check endianness]"
  write(6,'(1a)') "File, output, includes characters"
  write(6,'(1a)') "   and numbers with binary data."
  write(6,'("0x12345678= ",1i12)') ibuff 
  write(6,'("0xedcba988= ",1i12)') ibsgn
  write(6,'("A DP number:",1f12.5)') rbuff
  write(6,'(1a)') "Remark 1: Endianness only affects byte order. "
  write(6,'(1a)') "Since char is exactly 1 BYTE, it is the same as"
  write(6,'(1a)') "all endianness."
  write(6,'(1a)') "Remark 2: You can check everything, via hexdump&
 & or xxd."
  write(6,'(1a)') " (ex) hexdump -C -v output"
  write(6,'(1a)') "Remark 3: In typical Fortran compiler, endianness&
 & can be changed"
  write(6,'(1a)') "in several ways."
  write(6,'(1a)') "(ex) In compiling"
  write(6,'(1a)') "  gfortran -fconvert=big-endian foo.f90 -o foo.exe"
  write(6,'(1a)') "(ex) Use Environmental Variables"
  write(6,'(1a)') "     This depends on kinds of architecture."

  open(10,file='output',form='unformatted',access='stream') 
  write(10) cbuff
  write(10) -1
  write(10) ibuff
  write(10) ibsgn
  write(10) rbuff
  close(10)

  stop
end program main
