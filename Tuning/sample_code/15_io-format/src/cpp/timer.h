/* Copyright 2024 Research Organization for Information Science and Technology */
/* FYI
 * https://stackoverflow.com/questions/3789340/combining-c-and-c-how-does-ifdef-cplusplus-work
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#if defined(__cplusplus)
extern "C" {
#endif

/* Wall clock (Elapsed time) */
double get_elp_time();

/* CPU time */
double get_cpu_time();

/* Get resolution */
double get_elp_res () ;
double get_cpu_res () ;

#if defined(__cplusplus)
}
#endif

#endif /* __TIMER_H__ */
