/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef MYKERNEL_H
#define MYKERNEL_H
void saxpy_0(const int ns, float a, float * restrict x, float * restrict y);
void saxpy_1(const int ns, float a, float * restrict x, float * restrict y);
void saxpy_2(const int ns, float a, float * restrict x, float * restrict y);
int dummy(const int ns, float *a);
#endif /* MYKERNEL_H */
