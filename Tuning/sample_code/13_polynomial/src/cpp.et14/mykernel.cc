// Copyright 2023 Research Organization for Information Science and Technology
#include "mykernel.h"

void poly1 (vector_et<ARRAY_SIZE> &x ,
            vector_et<ARRAY_SIZE> &c0, vector_et<ARRAY_SIZE> &c1)
{
#ifdef __ICC_FORCEINLINE__
#pragma forceinline recursive // for icc
#endif // __ICC_FORCEINLINE__
   x = c0+x*c1;
}

void poly4 (vector_et<ARRAY_SIZE> &x ,
            vector_et<ARRAY_SIZE> &c0, vector_et<ARRAY_SIZE> &c1)
{
#ifdef __ICC_FORCEINLINE__
#pragma forceinline recursive // for icc
#endif // __ICC_FORCEINLINE__
   x = c0 
         +x*(c1 +x*(c0 +x*(c1 +x*c0))) ;
}

void poly8 (vector_et<ARRAY_SIZE> &x ,
            vector_et<ARRAY_SIZE> &c0, vector_et<ARRAY_SIZE> &c1)
{
#ifdef __ICC_FORCEINLINE__
#pragma forceinline recursive // for icc
#endif // __ICC_FORCEINLINE__
    x = c0 
          +x*(c1 +x*(c0 +x*(c1 +x*(c0 
	  +x*(c1 +x*(c0 +x*(c1 +x*c0 
          ))))))) ;
}

void poly16 (vector_et<ARRAY_SIZE> &x ,
            vector_et<ARRAY_SIZE> &c0, vector_et<ARRAY_SIZE> &c1)
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
