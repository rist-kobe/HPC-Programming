/* Copyright 2025 Research Organization for Information Science and Technology */
#pragma once

#define MIN(a, b) ( (a) < (b) ? (a) : (b) )

void mmp_simple (const int, double * restrict, 
   double * restrict, double * restrict);

void mmp_simple_omptarget (const int ns, double * restrict, 
  double * restrict, double * restrict);

void mmp_simple_omptarget_h1 (const int ns, double * restrict, 
  double * restrict, double * restrict);
  
void mmp_simple_omptarget_h2 (const int ns, double * restrict, 
  double * restrict, double * restrict);

void mmp_simple_blk (const int, const int, const int, 
   double * restrict, double * restrict, double * restrict);

void mmp_simple_va (const int, double mc[restrict][*], 
   double ma[restrict][*], double mb[restrict][*]);

void mmp_simple_blk_va (const int, const int, const int, 
   double mc[restrict][*], double ma[restrict][*], double mb[restrict][*]);

int dummy (const int, const int, double * restrict); 
