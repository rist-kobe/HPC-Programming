/* Copyright 2024 Research Organization for Information Science and Technology */
#include "mykernel.h"

void poly1 (double * restrict x, double * restrict c0, 
            double * restrict c1, const int ndim)
{
    for( int i=0; i<ndim; ++i ) {
      x[i] = c0[i]+x[i]*c1[i] ;
    }
}

void poly4 (double * restrict x, double * restrict c0, 
            double * restrict c1, const int ndim)
{
    for( int i=0; i<ndim; ++i ) {
      x[i] = c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              )))) ;
    }
}

void poly8 (double * restrict x, double * restrict c0, 
            double * restrict c1, const int ndim)
{
    for( int i=0; i<ndim; i++ ) {
      x[i] = c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              )))))))) ;
    }
}

void poly16 (double * restrict x, double * restrict c0, 
            double * restrict c1, const int ndim)
{
    for( int i=0; i<ndim; ++i ) {
      x[i] = c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              )))))))))))))))) ;
    }
}
