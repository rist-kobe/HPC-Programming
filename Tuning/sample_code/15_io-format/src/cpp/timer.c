/* Copyright 2024 Research Organization for Information Science and Technology */
/*----------------------------------------------------------------------
 *   timer.c                                                          
 *   Author:    Yukihiro Ota (yota@rist.or.jp)
 *--------------------------------------------------------------------*/
#include "timer.h"

/* These routines may NOT be compatible with -std=c99 and -std=c11 
   because clock_gettime is within the POSIX standard, rather than 
   the C standard.
   Several ways of using clock_gettime exist, depending on kinds of 
   compilers. Use of GNU extensions in Intel, e.g., -std=gnu99, works.
   Note: Some glibc (e.g., before 2.17) requires linking with -lrt.
   I suggest that you check manual of clock_gettime and your compiler 
   before the use.                                                    */

#include <time.h>

/* Wall clock (Elapsed time) */
#if ! defined(NOT_USE_REALTIME)
double get_elp_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_REALTIME, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}
#else
double get_elp_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_MONOTONIC, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}
#endif

/* CPU time */
double get_cpu_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_PROCESS_CPUTIME_ID, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}

/* Get resolution */
#if ! defined(NOT_USE_REALTIME)
double get_elp_res () {
  struct timespec res ;
  clock_getres ( CLOCK_REALTIME, &res ) ;
  return res.tv_sec*1.0e-6 + (double)res.tv_nsec*1.0e-3 ;
}
#else
double get_elp_res () {
  struct timespec res ;
  clock_getres ( CLOCK_MONOTONIC, &res ) ;
  return res.tv_sec*1.0e-6 + (double)res.tv_nsec*1.0e-3 ;
}
#endif

double get_cpu_res () {
  struct timespec res ;
  clock_getres ( CLOCK_PROCESS_CPUTIME_ID, &res ) ;
  return res.tv_sec*1.0e-6 + (double)res.tv_nsec*1.0e-3 ;
}

#if 0
/* Obsolete: Probably does not work                                  */

#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

/* Wall clock (Elapsed time) */
double get_elp_time () {
  struct timeval tv ;
  gettimeofday ( &tv, NULL ) ;
  return tv.tv_sec + (double)tv.tv_usec*1.0e-6 ;
}

/* CPU time */
double get_cpu_time () {
  struct rusage ru ;
  getrusage ( RUSAGE_SELF, &ru ) ;
  return ru.ru_utime.tv_sec + (double)ru.ru_utime.tv_sec*1.0e-6 ;
}

/* Get resolution */
double get_elp_res () {
  return -1.0; /* not implemented */
}

double get_cpu_res () {
  return -1.0; /* not implemented */
}
#endif
