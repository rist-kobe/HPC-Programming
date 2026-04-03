// Copyright 2025 Research Organization for Information Science and Technology
// Reference
// * OpenACC Getting started guid, 2.5 and 2.9;
//   https://docs.nvidia.com/hpc-sdk/compilers/openacc-gs/index.html
#pragma once

template <typename T>
class myvec
{
  public:
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

  myvec(const myvec &b) 
  {
    //for (int i=0; i<ns; ++i) v[i] = b.v[i] ;
    #pragma acc kernels loop present(this[0:1], b[0:1], b.v[0:ns], v[0:ns])
    for (int i=0; i<ns; ++i) {
       v[i] = b.v[i] ;
    }
  }

  myvec& operator =(myvec const& b) 
  {
    //for (int i=0; i<ns; ++i) v[i] = b.v[i] ;
    #pragma acc kernels loop present(this[0:1], b[0:1], b.v[0:ns], v[0:ns])
    for (int i=0; i<ns; ++i) {
       v[i] = b.v[i] ;
    }
    return *this ;
  }

  myvec& operator +=(myvec const& b) 
  {
    //for (int i=0; i<ns; ++i) v[i] += b.v[i] ;
    #pragma acc kernels loop present(this[0:1], b[0:1], b.v[0:ns], v[0:ns])
    for (int i=0; i<ns; ++i) {
       v[i] += b.v[i] ;
    }
    return *this ;
  }

  myvec& operator *=(myvec const& b) 
  {
    //for (int i=0; i<ns; ++i) v[i] *= b.v[i] ;
    #pragma acc kernels loop present(this[0:1], b[0:1], b.v[0:ns], v[0:ns])
    for (int i=0; i<ns; ++i) {
       v[i] *= b.v[i] ;
    }
    return *this ;
  }

  myvec operator+ (const myvec& b) 
  {
    // NOTE: Use of a temporal object (q) could be wrong from a performance point of view.
    // Never use your product run.
    myvec<T> q(ns) ;
    //for (int i=0; i<ns; ++i) q.v[i] = v[i] + b.v[i] ;
    #pragma acc kernels loop independent present(this[0:1], b[0:1], b.v[0:ns], v[0:ns])
    for (int i=0; i<ns; ++i) {
       q.v[i] = v[i] + b.v[i] ;
    }
    return q ;
  }

  myvec operator* (const myvec& b) 
  {
    // NOTE: Use of a temporal object (q) could be wrong from a performance point of view.
    // Never use your product run.
    myvec<T> q(ns) ;
    //for (int i=0; i<ns; ++i) q.v[i] = v[i] * b.v[i] ;
    #pragma acc kernels loop independent present(this[0:1], b[0:1], b.v[0:ns], v[0:ns])
    for (int i=0; i<ns; ++i) {
       q.v[i] = v[i] * b.v[i] ;
    }
    return q ;
  }

  #pragma acc routine gang
  void run_fma (myvec& a, myvec& b)
  {
     #pragma acc loop independent gang vector 
     for ( int i = 0; i < ns; ++i ) {
        v[i] = a.v[i] + v[i]*b.v[i];
     }
  }

  int dummy(const int itr)
  {
     int icon = 0;
     return icon;
  }

  private:
  int ns;
  T* v; 
};
