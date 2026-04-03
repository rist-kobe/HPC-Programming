/* Copyright 2024 Research Organization for Information Science and Technology */
void kernel_l3_s1 (int idummy, const int nsize, 
              double * restrict a, double * restrict b, double * restrict c,
              double * restrict u)
{
  /* 3 loads, 1  store */
  for (int i = 0; i < nsize; ++i) {
     u[i] = a[i] + b[i]*c[i];
  }
}

void kernel_l4_ls1 (int idummy, const int nsize, 
              double * restrict a, double * restrict b, 
              double * restrict c, double * restrict d, 
              double * restrict u)
{
  /* 4 loads, 1 load and store */
  for (int i = 0; i < nsize; ++i) {
     u[i] =  u[i] + d[i]*(a[i] + b[i]*c[i]);
  }
}

void kernel_l8_ls1 (int idummy, const int nsize, 
              double * restrict a, double * restrict b, 
              double * restrict c, double * restrict d,
              double * restrict e, double * restrict f,
              double * restrict g, double * restrict h,
	      double * restrict u)
{
  /* 8 loads, 1 load and store */
  for (int i = 0; i < nsize; ++i) {
     double u1 = d[i]*(a[i] + b[i]*c[i]);
     double u2 = h[i]*(e[i] + f[i]*g[i]);
     u[i] =  u[i] + u1 + u2; 
  }
}
