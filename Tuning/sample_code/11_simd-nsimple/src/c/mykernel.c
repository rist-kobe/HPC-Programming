/* Copyright 2024 Research Organization for Information Science and Technology */
#include "mykernel.h"
/*========================================================================
  vadd 
 =======================================================================*/
void vadd (const int ns, double * restrict va, double * restrict vb)
{
  for (int i=0; i<ns; ++i) 
    vb[i] += va[i];
}

/*========================================================================
  vadd_lsta: vadd with indirect access via an index list 
 =======================================================================*/
void vadd_lsta (const int ns, int * restrict ind, 
                double * restrict va, double * restrict vb)
{

#if defined(__USE_DIRECTIVES__)
#if defined(__INTEL_COMPILER)
  #pragma ivdep
  #pragma vector aligned
#elif defined(__clang__)
  #pragma clang loop vectorize(enable) interleave(enable) 
#elif defined(_OPENMP)
  #pragma omp simd 
#endif
#endif
  for (int i=0; i<ns; ++i ) {
     int ii = ind[i];
     vb[i] = vb[ii] + va[i];
  }
}

/*========================================================================
  vadd_aos: vadd with Array of Structure
 =======================================================================*/
void vadd_aos (const int ns, tcmp *v)
{
  for (int i=0; i<ns; ++i) 
    v[i].b += v[i].a;
}

/*========================================================================
  vadd_soa: vadd with Structure of Array
 =======================================================================*/
void vadd_soa (const int ns, tcmpv *v)
{
  for (int i=0; i<ns; ++i) 
    v->b[i] += v->a[i];
}

/*========================================================================
  vadd_while: vadd with while 
 =======================================================================*/
void vadd_while (const int ns, double * restrict va, double * restrict vb)
{
  int i;
  i = 0;
  while ( i < ns ) {
    vb[i] += va[i];
    i += 1;
  }
}
