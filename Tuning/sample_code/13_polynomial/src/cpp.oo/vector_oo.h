// Copyright 2024 Research Organization for Information Science and Technology 
#ifndef __VECTOR_OO_H__
#define __VECTOR_OO_H__

#include <array>

template <typename T, int N>
class vector_oo 
{
  public:

    vector_oo() {
      for (int i=0; i<N; ++i) v[i] = 0 ;
    }

    vector_oo(const vector_oo &b) {
      for (int i=0; i<N; ++i) v[i] = b.v[i] ;
    }

    vector_oo& operator =(vector_oo const& b) {
      for (int i=0; i<N; ++i) v[i] = b.v[i] ;
      return *this ;
    }

    vector_oo& operator +=(vector_oo const& b) {
      for (int i=0; i<N; ++i) v[i] += b.v[i] ; 
      return *this ;
    }

    vector_oo& operator *=(vector_oo const& b) {
      for (int i=0; i<N; ++i) v[i] *= b.v[i] ; 
      return *this ;
    }

    T& operator[] (int i) {
      return v[i] ;
    }

    const T& operator[] (int i) const {
      return v[i] ;
    }

    vector_oo operator+ (const vector_oo& b) {
      vector_oo<T,N> q ;
      for (int i=0; i<N; ++i) q.v[i] = v[i] + b.v[i] ;
      return q ;
    }

    vector_oo operator* (const vector_oo& b) {
      vector_oo<T,N> q ;
      for (int i=0; i<N; ++i) q.v[i] = v[i] * b.v[i] ;
      return q ;
    }

  ~vector_oo() {}

  private:
    std::array<T, N> v ; 
} ;

#endif // __VECTOR_OO_H__
