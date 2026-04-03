/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef __MYKERNEL_H__
#define __MYKERNEL_H__
void add(const int, double * restrict, double * restrict,
            double * restrict);
void daxpy(const int, double, double * restrict, double * restrict);
int dummy(const int, double *);
#endif /* __MYKERNEL_H__ */
