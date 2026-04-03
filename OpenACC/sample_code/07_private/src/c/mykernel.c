/* Copyright 2024 Research Organization for Information Science and Technology */
void func_0(const int ns, float * restrict A)
{
  int tmp[16];

  for (int i=0; i<ns; ++i) {
    for (int ii=0; ii<16; ++ii){
       tmp[ii] = (i+ii)%2; 
    }
    float s = 0.0f;
    for (int ii=0; ii<16; ++ii) {
       s += tmp[ii]*A[i];
    }
    A[i] = s;
  }
}

void func_1(const int ns, float * restrict A)
{
  int tmp[16];

  #pragma acc kernels
  {
  #pragma acc loop
  for (int i=0; i<ns; ++i) {
    for (int ii=0; ii<16; ++ii){
       tmp[ii] = (i+ii)%2; 
    }
    float s = 0.0f;
    for (int ii=0; ii<16; ++ii) {
       s += tmp[ii]*A[i];
    }
    A[i] = s;
  }
  } /* acc kernels */
}

void func_2(const int ns, float * restrict A)
{
  int tmp[16]; /* tmp[:] means tmp[0:16] */

  #pragma acc kernels
  {
  #pragma acc loop private(tmp[:])
  for (int i=0; i<ns; ++i) {
    for (int ii=0; ii<16; ++ii){
       tmp[ii] = (i+ii)%2; 
    }
    float s = 0.0f;
    for (int ii=0; ii<16; ++ii) {
       s += tmp[ii]*A[i];
    }
    A[i] = s;
  }
  } /* acc kernels */
}

void func_3(const int ns, float * restrict A)
{
  int tmp[16]; /* tmp[:] means tmp[0:16] */

  #pragma acc data copy(A[0:ns]) create(tmp[0:16])
  {
  #pragma acc kernels 
  {
  #pragma acc loop private(tmp)
  for (int i=0; i<ns; ++i) {
    #pragma acc loop 
    for (int ii=0; ii<16; ++ii){
       tmp[ii] = (i+ii)%2; 
    }
    float s = 0.0f;
    #pragma acc loop reduction(+:s)
    for (int ii=0; ii<16; ++ii) {
       s += tmp[ii]*A[i];
    }
    A[i] = s;
  }
  } /* acc kernels */
  } /* acc data */
}

int dummy(const int ns, float *a)
{
  return 0;
}

