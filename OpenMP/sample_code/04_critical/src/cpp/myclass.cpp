// Copyright 2024 Research Organization for Information Science and Technology 
#include <algorithm>
#include "myclass.h"

// Splits the range [0, size) into contiguous blocks, one per thread.
// The block of thread tid is the half-open range [ifrom, ito).
void MyClass::decomp_size(const int size, const int tid, const int nt, int &ifrom , int &ito)
{
   int delta = 1 + size / nt;
   ifrom = tid * delta;
   ito   = std::min(ifrom + delta, size);
}

// Indeed, the working array, u, is **NOT** needed.
// This implementation is nothing more than an example.
// Because u is a class member, it is SHARED among the threads; this
// data race is why the caller must protect func by a critical construct.
int MyClass::func(const int *a, const int ifrom, const int ito)
{
   int ret;
   int ifrom0, ifrom1;

   if ( (ifrom % 2) == 0 ) {
      ifrom0 = ifrom;
      ifrom1 = ifrom + 1;
   } else {
      ifrom0 = ifrom + 1;
      ifrom1 = ifrom;
   }

   u[0] = 0; 
   for (int i = ifrom0; i < ito; i +=2 ) {
      u[0] += a[i]; 
   }

   u[1] = 0;
   for (int i = ifrom1; i < ito; i +=2 ) {
      u[1] += -a[i]; 
   }

   ret = u[0] + u[1];
   return ret;
}

// A better implementation: the work array, u, is a local variable, so
// this function has no shared state and is safe to call concurrently.
int MyClass::func_local(const int *a, const int ifrom, const int ito) const
{
   int ret;
   int ifrom0, ifrom1;
   int u[2];

   if ( (ifrom % 2) == 0 ) {
      ifrom0 = ifrom;
      ifrom1 = ifrom + 1;
   } else {
      ifrom0 = ifrom + 1;
      ifrom1 = ifrom;
   }

   u[0] = 0; 
   for (int i = ifrom0; i < ito; i +=2 ) {
      u[0] += a[i]; 
   }

   u[1] = 0;
   for (int i = ifrom1; i < ito; i +=2 ) {
      u[1] += -a[i]; 
   }

   ret = u[0] + u[1];
   return ret;
}

