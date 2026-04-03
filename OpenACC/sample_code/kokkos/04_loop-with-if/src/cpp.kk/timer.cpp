// Copyright 2023 Research Organization for Information Science and Technology
#include <ctime>
#include "timer.h"

/* Wall clock */
double get_elp_time () {
  struct timespec tp ;
  clock_gettime ( CLOCK_REALTIME, &tp ) ;
  return  tp.tv_sec + (double)tp.tv_nsec*1.0e-9 ;
}
