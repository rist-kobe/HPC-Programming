/* Copyright 2025 Research Organization for Information Science and Technology */
#pragma once

#define MIN(a, b) ( (a) < (b) ? (a) : (b) )

#define noalias __restrict__

void mmp_simple (const int, const int, const int, 
  double * noalias, double * noalias, double * noalias);

void mmp_simple_acc (const int, const int, const int, 
  double * noalias, double * noalias, double * noalias);

void mmp_simple_acc_gv (const int, const int, const int, 
  double * noalias, double * noalias, double * noalias);

int dummy (const int, const int, double * noalias); 
