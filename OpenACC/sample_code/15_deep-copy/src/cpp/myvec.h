// Copyright 2025 Research Organization for Information Science and Technology
// Reference
// * OpenACC Getting started guid, 2.5 and 2.9;
//   https://docs.nvidia.com/hpc-sdk/compilers/openacc-gs/index.html
#pragma once

template <typename T>
class myvec
{
  public:
  T* v;

  myvec(const int ns_) 
  {
     ns = ns_;
     v = (T *)malloc ( ns*sizeof(T) );
     #pragma acc enter data copyin(this[0:1]) create(v[0:ns])
  }

  ~myvec() 
  {
     #pragma acc exit data delete(v[0:ns], this[0:1])
     free( v ); 
  }

  void update_host()
  {
     #pragma acc update self(v[0:ns])
  }

  void update_device()
  {
     #pragma acc update device(v[0:ns])
  }

  T& operator[] (int i) 
  {
    return v[i] ;
  }

  const T& operator[] (int i) const 
  {
    return v[i] ;
  }

  private:
  int ns;
};
