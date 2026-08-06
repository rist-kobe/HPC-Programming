/* Copyright 2023 Research Organization for Information Science and Technology */
#include <time.h>
#include "timer.h"

/* Default: CLOCK_MONOTONIC (safe for interval timing; unaffected by
   NTP adjustments or manual clock changes).
   Define USE_REALTIME to use CLOCK_REALTIME instead.               */
double get_elp_time()
{
  struct timespec tp;
#if ! defined(USE_REALTIME)
  if ( clock_gettime ( CLOCK_MONOTONIC, &tp ) != 0 ) return -1.0;
#else
  if ( clock_gettime ( CLOCK_REALTIME, &tp ) != 0 ) return -1.0;
#endif
  return tp.tv_sec + (double)tp.tv_nsec*1.0e-9;
}

double get_cpu_time()
{
  struct timespec tp;
  if ( clock_gettime ( CLOCK_PROCESS_CPUTIME_ID, &tp) != 0 ) return -1.0;
  return tp.tv_sec + (double)tp.tv_nsec*1.0e-9;
}
