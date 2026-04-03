// Copyright 2024 Research Organization for Information Science and Technology
#include "mykernel.h"

int dummy (double * _restrict_cpp x, int irep, double drep)
{
   int icon = 0;
   return icon;
}

void poly1 (double * _restrict_cpp x, double * _restrict_cpp c0, 
            double * _restrict_cpp c1, const int ndim)
{
    #pragma acc kernels loop independent \
    present(c0[0:ndim],c1[0:ndim],x[0:ndim])
    for( int i=0; i<ndim; ++i ) {
      x[i] = c0[i]+x[i]*c1[i] ;
    }
}

void poly4 (double * _restrict_cpp x, double * _restrict_cpp c0, 
            double * _restrict_cpp c1, const int ndim)
{
    #pragma acc kernels loop independent \
    present(c0[0:ndim],c1[0:ndim],x[0:ndim])
    for( int i=0; i<ndim; ++i ) {
      x[i] = c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              )))) ;
    }
}

void poly8 (double * _restrict_cpp x, double * _restrict_cpp c0, 
            double * _restrict_cpp c1, const int ndim)
{
    #pragma acc kernels loop independent \
    present(c0[0:ndim],c1[0:ndim],x[0:ndim])
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
    #pragma acc kernels loop independent \
    present(c0[0:ndim],c1[0:ndim],x[0:ndim])
    for( int i=0; i<ndim; ++i ) {
      x[i] = c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              )))))))))))))))) ;
    }
}

void poly32 (double * _restrict_cpp x, double * _restrict_cpp c0, 
            double * _restrict_cpp c1, const int ndim)
{
    #pragma acc kernels loop independent \
    present(c0[0:ndim],c1[0:ndim],x[0:ndim])
    for( int i=0; i<ndim; ++i ) {
      x[i] = c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              +x[i]*(c1[i] +x[i]*(c0[i] +x[i]*(c1[i] +x[i]*(c0[i]
              )))))))))))))))))))))))))))))))) ;
    }
}
