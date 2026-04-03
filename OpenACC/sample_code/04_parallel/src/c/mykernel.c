/* Copyright 2024 Research Organization for Information Science and Technology */
void add(const int ns, float * restrict a, 
            float * restrict b, float * restrict c)
{
#if ! defined(USE_UNIFIED_MEM)
  #pragma acc parallel present(a,b,c)
  #pragma acc loop 
#else
  #pragma acc parallel
  #pragma acc loop 
#endif
  for (int i =0; i<ns; ++i) {
    a[i] = b[i] + c[i];
  }
}

void saxpy(const int ns, float a, float * restrict x, 
           float * restrict y)
{
#if ! defined(USE_UNIFIED_MEM)
  #pragma acc parallel present(x,y) 
  #pragma acc loop 
#else
  #pragma acc parallel
  #pragma acc loop 
#endif
  for (int i =0; i<ns; ++i) {
    y[i] += a*x[i];
  }
}

int dummy(const int ns, float *a)
{
  return 0;
}

