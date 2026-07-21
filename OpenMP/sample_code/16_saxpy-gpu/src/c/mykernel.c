/* Copyright 2024 Research Organization for Information Science and Technology */
void saxpy_0(const int ns, float a, float * restrict x, 
           float * restrict y)
{
#if _OPENACC
  #pragma acc kernels
  #pragma acc loop
#endif
#if _OPENMP
  #pragma omp target 
  #pragma omp loop
#endif
  for (int i =0; i<ns; ++i) {
    y[i] += a*x[i];
  }
}

void saxpy_1(const int ns, float a, float * restrict x, 
           float * restrict y)
{
#if _OPENACC
  #pragma acc kernels copy(y[0:ns]) copyin(x[0:ns])
  #pragma acc loop
#endif
#if _OPENMP
  #pragma omp target map(tofrom:y[0:ns]) map(to:x[0:ns])
  #pragma omp loop
#endif
  for (int i =0; i<ns; ++i) {
    y[i] += a*x[i];
  }
}

int dummy(const int ns, float *a)
{
  return 0;
}

