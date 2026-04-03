// Copyright 2024 Research Organization for Information Science and Technology 
#ifndef __VECTOR_ET_H__
#define __VECTOR_ET_H__

#include <array>

template <typename LExp, typename BOp, typename RExp>
class Expression 
{
  public:
    Expression(const LExp& l, const RExp& r) : _l(l), _r(r) {} 

    inline double operator [](int index) const {
      return BOp::apply(_l[index], _r[index]);
    } 

  private:
    const LExp& _l ;
    const RExp& _r ;
} ;

struct plus_op {
  static double apply(const double& a, const double& b) { return a+b; }
} ;

struct mult_op {
  static double apply(const double& a, const double& b) { return a*b; }
} ;

template <typename T, int N>
class vector_et
{
  public:
    vector_et() { 
      int i ;
      for( i=0; i<N; ++i ) v[i] = 0 ;
    } 

    vector_et(const vector_et& b) {
      int i ;
      for ( i=0; i<N; ++i) v[i] = b.v[i] ;
    }

    vector_et& operator =(const vector_et& b) {
      int i ;
      for ( i=0; i<N; ++i ) v[i] = b.v[i] ;
      return *this ;
    }

    template<typename L, typename OP, typename R>
    vector_et(const Expression<L,OP,R>& b) {
      int i ;
      for( i=0; i<N; ++i ) v[i] = b[i] ;
    }

    template<typename R>
    vector_et& operator =(const R& b) {
      int i ;
      for ( i=0; i<N; ++i) v[i] = b[i] ;
      return *this ;
    }

    T& operator[] (int i) {
      return v[i] ;
    }

    const T& operator[] (int i) const {
      return v[i] ;
    }
    
    ~vector_et() {}

  private:
    std::array<T,N> v ;

} ;

template <typename L, typename R>
inline Expression<L, plus_op, R> 
operator +(const L& lhs, const R& rhs) {
  return Expression<L, plus_op, R>(lhs,rhs) ;
}

template <typename L, typename R>
inline Expression<L, mult_op, R> 
operator *(const L& lhs, const R& rhs) {
  return Expression<L, mult_op, R>(lhs,rhs) ;
}

#endif /* __VECTOR_ET_H__ */
