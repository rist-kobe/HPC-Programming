/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef __MYKERNEL_H__
#define __MYKERNEL_H__

void vadd (const int, double * restrict, double * restrict);
void vadd_raw (const int, double * restrict, double * restrict);
void vadd_war (const int, double * restrict, double * restrict);
#if defined(_OPENMP)
void vadd_ompsimd (const int, double *, double *);
#endif

#endif /* __MYKERNEL_H__ */
