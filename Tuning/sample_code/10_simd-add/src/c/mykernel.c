/* Copyright 2024 Research Organization for Information Science and Technology */
/*========================================================================
  vadd 
 =======================================================================*/
void vadd (const int ns, double * restrict va, double * restrict vb)
{
  for (int i=0; i<ns; ++i) 
    vb[i] += va[i];
}

/*========================================================================
  vadd_raw: read-after-write (flow) dependence
   vb[1] = vb[0] + va[1]
   vb[2] = vb[1] + va[2]
   vb[3] = vb[2] + va[3]
 =======================================================================*/
void vadd_raw (const int ns, double * restrict va, double * restrict vb)
{
  for (int i=1; i<ns; ++i)
    vb[i] = vb[i-1] + va[i]; 
}

/*========================================================================
  vadd_war: write-after-read (anti-flow) dependence
   vb[0] = vb[1] + va[0]
   vb[1] = vb[2] + va[1]
   vb[2] = vb[1] + va[2]
 =======================================================================*/
void vadd_war (const int ns, double * restrict va, double * restrict vb)
{
  for (int i=0; i<ns-1; ++i)
    vb[i] = vb[i+1] + va[i]; 
}


#if defined(_OPENMP)
/*========================================================================
  vadd_ompsimd 
 =======================================================================*/
void vadd_ompsimd (const int ns, double * va, double *vb)
{
  #pragma omp simd
  for (int i=0; i<ns; ++i) {
    vb[i] += va[i];
  }
}
#endif
