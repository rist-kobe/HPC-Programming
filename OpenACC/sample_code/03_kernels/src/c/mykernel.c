/* Copyright 2025 Research Organization for Information Science and Technology */
void saxpy_0(const int ns, float a, float * restrict x, 
           float * restrict y)
{
  #pragma acc kernels
  for (int i =0; i<ns; ++i) {
    y[i] += a*x[i];
  }
}

void saxpy_omp_0(const int ns, float a, float * restrict x, 
           float * restrict y)
{
  #pragma omp parallel for schedule(static)
  for (int i =0; i<ns; ++i) {
    y[i] += a*x[i];
  }
}

void saxpy_1(const int ns, float a, float * restrict x, 
           float * restrict y)
{
  #pragma acc data copy(y[0:ns]) copyin(x[0:ns])
  #pragma acc kernels
  for (int i =0; i<ns; ++i) {
    y[i] += a*x[i];
  }
}

int dummy(const int ns, float *a)
{
  return 0;
}

