! Copyright 2024 Research Organization for Information Science and Technology
module common_var
  integer,parameter :: NSIZE=4

  contains
  integer function func_a (tid,nt)
    integer :: tid,ttid,nt
    ttid = tid - 1
    func_a = (nt + ttid*(nt + ttid*(nt + ttid*(nt + ttid))))
  end function func_a

  integer function func_b (ns,tid,nt,a)
    integer :: ns,tid,ttid,nt
    integer :: a(1:ns)
    integer :: ind
    ttid = tid - 1
    ind = tid + 2
    if ( ind > ns ) then
       ind = ind - ns
    end if
    func_b = ttid*nt + a(ind)
  end function func_b
end module common_var 

program main
  use common_var,only: NSIZE, func_a, func_b
  use omp_lib
  implicit none

  integer :: i,tid, nthreads
  integer :: a(NSIZE)
  integer :: b(NSIZE)

  nthreads = omp_get_max_threads ()
  if ( nthreads .ne. NSIZE ) then
    write(6,'("[ERROR] Number of threads must be equal to 4")')
    stop
  end if

  ! w/ omp_barrier 
  a(:) = 0
  b(:) = 0

  ! [Note] num_threads clasue
  ! num_threads(4) means the number of threads is forced 
  ! to be 4 in the corresponding parallel region.       
  !$omp parallel private(tid,nthreads) &
  !$omp & num_threads(4)
  tid = omp_get_thread_num () + 1
  nthreads = omp_get_num_threads ()
  a(tid) = func_a (tid, nthreads)
  !$omp barrier
  b(tid) = func_b (NSIZE, tid, nthreads, a)
  !$omp end parallel

  write(6,'("[With omp_barrier]")')
  do i = 1, NSIZE
    write(6,'("a(",1I2,")= ",1I4,1x,&
&        "b(",1I2,")=,"1I4)') i,a(i),i,b(i)
  end do

  ! w/o omp_barrier 
  a(:) = 0
  b(:) = 0

  !$omp parallel private(tid,nthreads) &
  !$omp & num_threads(4)
  tid = omp_get_thread_num () + 1
  nthreads = omp_get_num_threads ()
  a(tid) = func_a (tid, nthreads)
  ! /* func_b requires full update of a */
  b(tid) = func_b (NSIZE, tid, nthreads, a)
  !$omp end parallel

  write(6,'("[Without omp_barrier]")')
  do i = 1, NSIZE
    write(6,'("a(",1I2,")= ",1I4,1x,&
&        "b(",1I2,")=,"1I4)') i,a(i),i,b(i)
  end do

  stop
end program main
