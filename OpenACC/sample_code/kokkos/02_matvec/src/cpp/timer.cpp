/* Copyright 2024 Research Organization for Information Science and Technology */
#include <ctime>
#include "timer.hpp"

double get_elp_time()
{
  struct timespec tp;
  clock_gettime ( CLOCK_REALTIME, &tp );
  return tp.tv_sec + (double)tp.tv_nsec*1.0e-9;
}

double get_cpu_time()
{
  struct timespec tp;
  clock_gettime ( CLOCK_PROCESS_CPUTIME_ID, &tp);
  return tp.tv_sec + (double)tp.tv_nsec*1.0e-9;
}
