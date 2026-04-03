// Copyright 2024 Research Organization for Information Science and Technology 
#include <cmath>
#include "myclass.h"

void MyClass::decomp_size(const int size, int tid, int nt, int &ifrom , int &ito)
{
   int delta = 1 + size / nt;
   ifrom = tid * delta;
   ito   = ((ifrom + delta) > size ) ? size : ifrom + delta;
}

// Indeed, the working array, u, is **NOT** needed.
// This implementation is nothing more than an example.
int MyClass::func(int *a, int ifrom, int ito)
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

