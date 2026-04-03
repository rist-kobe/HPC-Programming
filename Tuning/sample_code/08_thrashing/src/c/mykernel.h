/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef MYKERNEL_H
#define MYKERNEL_H
void kernel_l3_s1 (int idummy, const int nsize, 
              double * restrict , double * restrict , double * restrict ,
              double * restrict);

void kernel_l4_ls1 (int, const int , 
              double * restrict , double * restrict , 
              double * restrict , double * restrict ,
              double * restrict);

void kernel_l8_ls1 (int, const int , 
              double * restrict , double * restrict , 
              double * restrict , double * restrict ,
              double * restrict , double * restrict ,
              double * restrict , double * restrict ,
              double * restrict);

#endif /* MYKERNEL_H */
