/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef __MYKERNEL_H__
#define __MYKERNEL_H__

#define MIN(a, b) ( (a) < (b) ? (a) : (b) )

void mmp_simple(const int ns, double mc[restrict][ns],
   double ma[restrict][ns], double mb[restrict][ns]);

void mmp_simple_blk(const int ns, const int nbk1, const int nbk2,
  double mc[restrict][ns], double ma[restrict][ns], double mb[restrict][ns]);

void mmp_lex_tp_blk(const int ns, const int nbk1, const int nbk2,
  const int nbkr, const int nbkc,
  double mc[restrict][ns], double ma[restrict][ns], double mb[restrict][ns],
  double work[restrict][ns]);

void trans(const int nrow, const int ncol, double tm[restrict][nrow],
  double m[restrict][ncol]);

void trans_blk(const int nrow, const int ncol, const int nbkr, const int nbkc,
  double tm[restrict][nrow], double m[restrict][ncol]);

int dummy(const int ns, double mat[restrict][ns]);
#endif /* __MYKERNEL_H__ */
