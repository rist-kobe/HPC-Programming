// Copyright 2024 Research Organization for Information Science and Technology
#include "mykernel.h"

void poly1 (double * _restrict_cpp x, double * _restrict_cpp c0, 
            double * _restrict_cpp c1, const int ndim)
{
    for( int i=0; i<ndim; ++i ) {
      x[i] = c0[i]+x[i]*c1[i] ;
    }
}

void poly4 (double * _restrict_cpp x, double * _restrict_cpp c0, 
            double * _restrict_cpp c1, const int ndim)
{
    for( int i=0; i<ndim; ++i ) {
      x[i] = c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              )))) ;
    }
}

void poly8 (double * _restrict_cpp x, double * _restrict_cpp c0, 
            double * _restrict_cpp c1, const int ndim)
{
    for( int i=0; i<ndim; i++ ) {
      x[i] = c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              )))))))) ;
    }
}

void poly16 (double * _restrict_cpp x, double * _restrict_cpp c0, 
            double * _restrict_cpp c1, const int ndim)
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
