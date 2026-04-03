! Copyright 2024 Research Organization for Information Science and Technology
!-----------------------------------------------------------------------
!  By-hand timer and Use of gprof 
!  Author:      Yukihiro Ota (yota@rist.or.jp)
!-----------------------------------------------------------------------
program main

  implicit none

  integer,parameter :: nn = 20000
  integer :: i, k

  real(8) :: a(nn)

#ifdef USE_ELP_TIMER
  integer :: ielp1, ielp2, icount_rate, icount_max
  real(8) :: elp
#endif
#ifdef USE_CPU_TIMER
  real(8) :: cpu1, cpu2
#endif

! set values
  do i = 1, nn
    a(i) = 0.d0
  enddo

! routine 1
#ifdef USE_ELP_TIMER
  call system_clock ( ielp1, icount_rate, icount_max )
#endif
#ifdef USE_CPU_TIMER
  call cpu_time ( cpu1 )
#endif
  do k = 1, 100000
    call sub1 ( a, nn ) 
  enddo
#ifdef USE_ELP_TIMER
  call system_clock ( ielp2, icount_rate, icount_max )
  if ( ielp1 <= ielp2 ) then
    elp = (ielp2 - ielp1) / dble(icount_rate)
  else
    elp = (ielp2 - ielp1 + icount_max + 1) / dble(icount_rate)
  endif
  write (6,'(1a24,1f13.6)') "Elapsed time (sec)     ", elp
#endif
#ifdef USE_CPU_TIMER
  call cpu_time ( cpu2 )
  write (6,'(1a24,1f13.6)') "CPU time (sec)         ", cpu2 - cpu1 
#endif
  write (6,'(1a24,1f13.6)') "a(1) =                 ", a(1)

! routine 2
#ifdef USE_ELP_TIMER
  call system_clock ( ielp1, icount_rate, icount_max )
#endif
#ifdef USE_CPU_TIMER
  call cpu_time ( cpu1 )
#endif
  do k = 1, 200000
    call sub2 ( a, nn ) 
  enddo
#ifdef USE_ELP_TIMER
  call system_clock ( ielp2, icount_rate, icount_max )
  if ( ielp1 <= ielp2 ) then
    elp = (ielp2 - ielp1) / dble(icount_rate)
  else
    elp = (ielp2 - ielp1 + icount_max + 1) / dble(icount_rate)
  endif
  write (6,'(1a24,1f13.6)') "Elapsed time (sec)     ", elp
#endif
#ifdef USE_CPU_TIMER
  call cpu_time ( cpu2 )
  write (6,'(1a24,1f13.6)') "CPU time (sec)         ", cpu2 - cpu1 
#endif
  write (6,'(1a24,1f13.6)') "a(1) =                 ", a(1)

! finalization
  stop
end
!-----------------------------------------------------------------------
!   sub 1
!-----------------------------------------------------------------------
subroutine sub1 ( a, nn )
   implicit none
   real(8),intent(inout) :: a(nn)
   integer,intent(in) :: nn
   !*** local variables ***
   integer :: i, k

   do i = 1, nn
     a(i) = a(i) + 1.d0
   enddo

   do k = 1, 2
     call sub3 ( a, nn )
   enddo

   return
end subroutine 
!-----------------------------------------------------------------------
!   sub 2
!-----------------------------------------------------------------------
subroutine sub2 ( a, nn )
   implicit none
   real(8),intent(inout) :: a(nn)
   integer,intent(in) :: nn
   !*** local variables ***
   integer :: i, k

   a(1) = sin(a(1))

   do i = 1, nn
     a(i) = a(i) + 1.d0
   enddo

   do k = 1, 4
     call sub3 ( a, nn )
   enddo

   return
end subroutine 
!-----------------------------------------------------------------------
!   sub 3
!-----------------------------------------------------------------------
subroutine sub3 ( a, nn )
   implicit none
   real(8),intent(inout) :: a(nn)
   integer,intent(in) :: nn
   !*** local variables ***
   integer :: i

   a(1) = sin(a(1))

   do i = 1, nn
     a(i) = a(i) + 1.d0
   enddo

   return 
end subroutine 
