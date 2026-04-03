/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef __MYKERNEL_H__
#define __MYKERNEL_H__

#include "mytype.h"

void vadd (const int, double * restrict, double * restrict);
void vadd_lsta (const int, int * restrict, 
                double * restrict, double * restrict);
void vadd_aos (const int, tcmp *);
void vadd_soa (const int, tcmpv *);
void vadd_while (const int, double * restrict, double * restrict);

#endif /* __MYKERNEL_H__ */
