! Copyright 2024 Research Organization for Information Science and Technology
!-----------------------------------------------------------------------
! Some 'benchmark' of multiplication and addition
! Authors:            Yukihiro Ota (yota@rist.or.jp)
! Last Update:        4 Dec. 2018
!-----------------------------------------------------------------------
      program main
        use mytype, only: DP
        use mykernel, only: poly1, poly4, poly8, poly16
        implicit none
! #define ARRAY_SIZE 10000
#define ARRAY_SIZE 1000
        !---------------------------------------------------------------
        ! local variables
        !---------------------------------------------------------------
        integer,parameter :: nrep   = 100000 
        integer,parameter :: ndim   = ARRAY_SIZE 
        integer :: i,irep
        integer :: seedsize
        integer :: ielp1, ielp2, icount_rate, icount_max
        integer,allocatable :: seed(:)

        real(kind=DP) :: tmp1,tmp2 
        real(kind=DP) :: drep
        real(kind=DP) :: flop, mem 
        real(kind=DP) :: x(1:ndim) 
        real(kind=DP) :: c1(1:ndim) , c2(1:ndim)
        real(kind=DP) :: elp, flops

        !---------------------------------------------------------------
        ! set coefficients
        !---------------------------------------------------------------
        drep = 1.0_DP/nrep
        call system_clock(count=irep)
        call random_seed(size=seedsize) 
        allocate( seed(seedsize) )
        call random_seed(get=seed)
        do i=1,seedsize
          seed(i) = irep 
        enddo
        call random_seed(put=seed)
        do i=1,ndim
          call random_number(tmp1) 
          call random_number(tmp2) 
          c1(i)  = 2.0_DP*(tmp1-0.5_DP)*drep
          c2(i)  = 2.0_DP*(tmp2-0.5_DP)*drep/2.0_DP
        enddo
        deallocate( seed ) 
        write(6,'(1a)')                                                &
          "------------------------------------------------------"     
        write(6,'(1a)')                                                &
          "Some benchmark of multiplication and addition" 
        write(6,'(1a)')                                                &
          "Authors:       Yukihiro Ota (yota@rist.or.jp)" 
        write(6,'(1a)')                                                &
          "               Tatsunobu Kokubo"
        write(6,'(1a)')                                                &
          "Last Update:   11 Oct 2017"
        write(6,'(1a)')                                                &
          "[setup] "
        write(6,'(1a28,1i10)')                                         &
          "Number of repetitions :", nrep
        write(6,'(1a28,1i10)')                                         &
          "Number of elements    :", ndim 
        write(6,'(1a)')                                                &
          "------------------------------------------------------"     
        !---------------------------------------------------------------
        ! This kernel is made according to an idea in 9th Degree 
        ! Polynomial(inside mod1a) of EuroBen 
        ! (https://www.euroben.nl/index.php)
        !---------------------------------------------------------------
        !! 1 FMA 
        x = 0.0_DP 
        flop = 2.0_DP*ndim*nrep
        mem  = 3.0_DP*DP*ndim
        write(6,'(1a28)')        "[run] 1 FMA            "
        write(6,'(1a28,1f20.9)') "Giga FLOP             :",flop*1.0e-9_DP
        write(6,'(1a28,1f20.9)') "Memory (MB)           :",mem*1.0e-6_DP

        call system_clock(ielp1,icount_rate,icount_max)
        do irep=1,nrep
          call poly1(ndim, x, c1, c2)
          x(1)=x(1)+drep*irep !! prevent optimization by smart compiler
        enddo
        call system_clock(ielp2,icount_rate,icount_max)

        if ( ielp1 .le. ielp2 ) then
          elp=(ielp2-ielp1)/dble(icount_rate)
        else
          elp=(ielp2-ielp1+icount_max+1)/dble(icount_rate)
        endif
        flops = 1.0e-9_DP*flop/elp !! Giga flop/s 
        write(6,'(1a28,1f10.4)') "Elapsed (sec.)        :",elp
        write(6,'(1a28,1f10.4)') "Giga FLOPs            :",flops
        write(6,'(1a12,1f22.8)') "x(1)=",x(1)  

        !! 4 FMA 
        x = 0.0_DP
        flop = 8.0_DP*ndim*nrep
        mem  = 3.0_DP*DP*ndim
        write(6,'(1a28)')        "[run] 4 FMA            "
        write(6,'(1a28,1f20.9)') "Giga FLOP             :",flop*1.e-9_DP
        write(6,'(1a28,1f20.9)') "Memory (MB)           :",mem*1.e-6_DP
        
        call system_clock(ielp1,icount_rate,icount_max)
        do irep=1,nrep
          call poly4(ndim, x, c1, c2)
          x(1)=x(1)+drep*irep !! prevent optimization by smart compiler
        enddo
        call system_clock(ielp2,icount_rate,icount_max)

        if ( ielp1 .le. ielp2 ) then
          elp=(ielp2-ielp1)/dble(icount_rate)
        else
          elp=(ielp2-ielp1+icount_max+1)/dble(icount_rate)
        endif
        flops = 1.0e-9_DP*flop/elp !! Giga flop/s 
        write(6,'(1a28,1f10.4)') "Elapsed (sec.)        :",elp
        write(6,'(1a28,1f10.4)') "Giga FLOPs            :",flops
        write(6,'(1a12,1f22.8)') "x(1)=",x(1)  
        
        !! 8 FMA 
        x = 0.0_DP
        flop = 16.0_DP*ndim*nrep
        mem  = 3.0_DP*DP*ndim
        write(6,'(1a28)')        "[run] 8 FMA            "
        write(6,'(1a28,1f20.9)') "Giga FLOP             :",flop*1.0e-9_DP
        write(6,'(1a28,1f20.9)') "Memory (MB)           :",mem*1.0e-6_DP

        call system_clock(ielp1,icount_rate,icount_max)
        do irep=1,nrep
          call poly8(ndim, x, c1, c2)
          x(1)=x(1)+drep*irep !! prevent optimization by smart compiler
        enddo
        call system_clock(ielp2,icount_rate,icount_max)

        if ( ielp1 .le. ielp2 ) then
          elp=(ielp2-ielp1)/dble(icount_rate)
        else
          elp=(ielp2-ielp1+icount_max+1)/dble(icount_rate)
        endif
        flops = 1.0e-9_DP*flop/elp !! Giga flop/s 
        write(6,'(1a28,1f10.4)') "Elapsed (sec.)        :",elp
        write(6,'(1a28,1f10.4)') "Giga FLOPs            :",flops
        write(6,'(1a12,1f22.8)') "x(1)=",x(1)  

        !! 16 FMA 
        x = 0.0_DP
        flop = 32.0_DP*ndim*nrep
        mem  = 3.0_DP*DP*ndim
        write(6,'(1a28)')        "[run] 16 FMA           "
        write(6,'(1a28,1f20.9)') "Giga FLOP             :",flop*1.0e-9_DP
        write(6,'(1a28,1f20.9)') "Memory (MB)           :",mem*1.0e-6_DP

        call system_clock(ielp1,icount_rate,icount_max)
        do irep=1,nrep
          call poly16(ndim, x, c1, c2)
          x(1)=x(1)+drep*irep !! prevent optimization by smart compiler
        enddo
        call system_clock(ielp2,icount_rate,icount_max)

        if ( ielp1 .le. ielp2 ) then
          elp=(ielp2-ielp1)/dble(icount_rate)
        else
          elp=(ielp2-ielp1+icount_max+1)/dble(icount_rate)
        endif
        flops = 1.0e-9_DP*flop/elp !! Giga flop/s 
        write(6,'(1a28,1f10.4)') "Elapsed (sec.)        :",elp
        write(6,'(1a28,1f10.4)') "Giga FLOPs            :",flops
        write(6,'(1a12,1f22.8)') "x(1)=",x(1)  

        stop
      end program main
