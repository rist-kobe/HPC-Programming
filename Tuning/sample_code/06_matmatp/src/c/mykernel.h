/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef __MYKERNEL_H__
#define __MYKERNEL_H__

#define MIN(a, b) ( (a) < (b) ? (a) : (b) )

void mmp_simple(const int, double mc[restrict][*], 
   double ma[restrict][*], double mb[restrict][*]);

void mmp_simple_blk(const int, const int, const int, 
  double mc[restrict][*], double ma[restrict][*], double mb[restrict][*]);

void mmp_lex_tp_blk(const int, const int, const int, const int, const int, 
  double mc[restrict][*], double ma[restrict][*], double mb[restrict][*]);

void trans(const int, const int, double tm[restrict][*], 
  double m[restrict][*]);

void trans_blk(const int, const int, const int, const int , 
  double tm[restrict][*], double m[restrict][*]);

int dummy(const int, double mat[restrict][*]);
#endif /* __MYKERNEL_H_ */
