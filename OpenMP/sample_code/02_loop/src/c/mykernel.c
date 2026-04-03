/* Copyright 2024 Research Organization for Information Science and Technology */
void add(const int ns, double * restrict a, 
            double * restrict b, double * restrict c)
{
  int i;
  #pragma omp parallel for private(i) \
  shared(a,b,c) schedule(static)
  for (i =0; i<ns; ++i) {
    a[i] = b[i] + c[i];
  }
}

void daxpy(const int ns, double a, double * restrict x, 
           double * restrict y)
{
  int i;
  for (i =0; i<ns; ++i) {
    y[i] += a*x[i];
  }
}

int dummy(const int ns, double *a)
{
  return 0;
}

