/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef MYKERNEL_H
#define MYKERNEL_H

void coremgs(double * restrict, double * restrict, 
            const int, const int);
void coremgs_swp(double * restrict, double * restrict, 
            const int, const int);
int dummy(double * restrict, const int);

#endif /* MYKERNEL_H */
