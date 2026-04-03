// Copyright 2024 Research Organization for Information Science and Technology
#include "mykernel.h"

void poly1 (vector_oo<double,ARRAY_SIZE> &x ,
            vector_oo<double,ARRAY_SIZE> &c0, vector_oo<double,ARRAY_SIZE> &c1)
{
#ifdef __ICC_FORCEINLINE__
#pragma forceinline recursive // for icc
#endif // __ICC_FORCEINLINE__
   x = c0+x*c1;
}

void poly4 (vector_oo<double,ARRAY_SIZE> &x ,
            vector_oo<double,ARRAY_SIZE> &c0, vector_oo<double,ARRAY_SIZE> &c1)
{
#ifdef __ICC_FORCEINLINE__
#pragma forceinline recursive // for icc
#endif // __ICC_FORCEINLINE__
   x = c0 
         +x*(c1 +x*(c0 +x*(c1 +x*c0))) ;
}

void poly8 (vector_oo<double,ARRAY_SIZE> &x ,
            vector_oo<double,ARRAY_SIZE> &c0, vector_oo<double,ARRAY_SIZE> &c1)
{
#ifdef __ICC_FORCEINLINE__
#pragma forceinline recursive // for icc
#endif // __ICC_FORCEINLINE__
    x = c0 
          +x*(c1 +x*(c0 +x*(c1 +x*(c0 
	  +x*(c1 +x*(c0 +x*(c1 +x*c0 
          ))))))) ;
}

void poly16 (vector_oo<double,ARRAY_SIZE> &x ,
            vector_oo<double,ARRAY_SIZE> &c0, vector_oo<double,ARRAY_SIZE> &c1)
{
#ifdef __ICC_FORCEINLINE__
#pragma forceinline recursive // for icc
#endif // __ICC_FORCEINLINE__
    x = c0 
          +x*(c1 +x*(c0 +x*(c1 +x*(c0
	  +x*(c1 +x*(c0 +x*(c1 +x*(c0 
	  +x*(c1 +x*(c0 +x*(c1 +x*(c0 
	  +x*(c1 +x*(c0 +x*(c1 +x*c0 
          ))))))))))))))) ;
}
