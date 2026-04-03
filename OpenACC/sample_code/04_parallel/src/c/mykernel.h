/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef __MYKERNEL_H__
#define __MYKERNEL_H__
void add(const int, float * restrict, float * restrict,
            float * restrict);
void saxpy(const int, float, float * restrict, float * restrict);
int dummy(const int, float *);
#endif /* __MYKERNEL_H__ */
