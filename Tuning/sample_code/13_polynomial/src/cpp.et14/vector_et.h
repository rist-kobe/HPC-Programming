// Copyright 2024 Research Organization for Information Science and Technology 
#ifndef __VECTOR_ET_H__
#define __VECTOR_ET_H__

#include <type_traits>
#include <array>

template <typename T> struct plus_op ; 
template <typename T> struct mult_op ;

template <
	typename LeftExp,
	typename BinaryOp,
	typename RightExp
>
class vector_expression
{
  public:
	vector_expression() = delete;
	vector_expression(LeftExp _l, RightExp _r) :
	    l( std::forward<LeftExp >(_l) ),
	    r( std::forward<RightExp>(_r) )
	{}

    // Prohibit copying...
    vector_expression(vector_expression const&) = delete;
    vector_expression& operator =(vector_expression const&) = delete;

    // Allow moves...
    vector_expression(vector_expression&&) = default;
    vector_expression& operator =(vector_expression&&) = default;

    /* Expression for addition */
    template <typename RE>
    inline auto operator+ (RE&& re) const -> decltype(auto) {
      return 
        vector_expression<
          vector_expression<LeftExp,BinaryOp,RightExp> const&,
          plus_op<decltype(std::forward<RE>(re))>,
          decltype(std::forward<RE>(re))
        >(*this, std::forward<RE>(re)) ;
    }
    /* Expression for multiplication */
    template <typename RE>
    inline auto operator* (RE&& re) const -> decltype(auto) {
      return 
        vector_expression<
          vector_expression<LeftExp,BinaryOp,RightExp> const&,
          mult_op<decltype(std::forward<RE>(re))>,
          decltype(std::forward<RE>(re))
        >(*this, std::forward<RE>(re)) ;
    }

    inline auto operator [](std::size_t index) const -> decltype(auto) {
      return BinaryOp::apply(l[index], r[index]);
    }

  private:
    LeftExp l;
    RightExp r;


};

template <typename T> struct plus_op {
  static T apply(T const& a, T const& b)    { return a + b; }
};
template <typename T> struct mult_op {
  static T apply(T const& a, T const& b)    { return a * b; }
};

template <std::size_t N>
class vector_et
{
  using impl_type = std::array<double, N>;

  public:
    typedef typename impl_type::value_type value_type;

    vector_et()
    {
      for (std::size_t i=0; i<N; ++i)
          v[i] = 0;
    }

    vector_et(vector_et const& mv) noexcept
    {
      using namespace std;
      copy(begin(mv.v), end(mv.v), begin(v));
    }

    vector_et(vector_et&& mv) noexcept
    {
      using namespace std;
      move(begin(mv.v), end(mv.v), begin(v));
    }

    vector_et(std::initializer_list<value_type> l)
    {
      using namespace std;
      copy(begin(l), end(l), begin(v));
    }

    vector_et& operator =(vector_et const& mv) noexcept
    {
      using namespace std;
      copy(begin(mv.v), end(mv.v), begin(v));
      return *this;
    }

    vector_et& operator =(vector_et&& mv) noexcept
    {
      using namespace std;
      move(begin(mv.v), end(mv.v), begin(v));
      return *this;
    }

    ~vector_et(){}

    void swap(vector_et& mv)
    {
      using namespace std;
      for (size_t i=0; i<N; ++i)
        swap(v[i], mv[i]);
    }

    auto operator [](std::size_t index) const
      -> decltype(auto)
    {
      return v[index];
    }

    auto operator [](std::size_t index)
      -> decltype(auto)
    {
      return v[index];
    }

    inline vector_et& operator +=(vector_et const& b)
    {
      using namespace std;
      for (size_t i=0; i<N; ++i)
        v[i] += b[i];
      return *this;
    }
    inline vector_et& operator *=(vector_et const& b)
    {
      using namespace std;
      for (size_t i=0; i<N; ++i)
        v[i] *= b[i];
      return *this;
    }

    template <typename LE, typename Op, typename RE>
    inline vector_et(vector_expression<LE,Op,RE>&& mve)
    {
      using namespace std;
      for (size_t i=0; i < N; ++i)
        v[i] = mve[i];
    }

    template <typename RightExpr>
    inline vector_et& operator =(RightExpr&& re)
    {
      using namespace std;
      for (size_t i=0; i<N; ++i)
        v[i] = re[i];
      return *this;
    }

    template <typename RightExpr>
    inline vector_et& operator +=(RightExpr&& re)
    {
      using namespace std;
      for (size_t i=0; i<N; ++i)
        v[i] += re[i];
      return *this;
    }
    template <typename RightExpr>
    inline vector_et& operator *=(RightExpr&& re)
    {
      using namespace std;
      for (size_t i=0; i<N; ++i)
        v[i] *= re[i];
      return *this;
    }

    template <typename RightExpr>
    inline auto operator +(RightExpr&& re) const -> decltype(auto)
    {
      return
        vector_expression<
          vector_et const&,
          plus_op<value_type>,
          decltype(std::forward<RightExpr>(re))
        >( *this, std::forward<RightExpr>(re) );
    }
    template <typename RightExpr>
    inline auto operator *(RightExpr&& re) const -> decltype(auto)
    {
      return
        vector_expression<
          vector_et const&,
          mult_op<value_type>,
          decltype(std::forward<RightExpr>(re))
        >( *this, std::forward<RightExpr>(re) );
    }


  private:
    impl_type v;
};

#endif // __VECTOR_ET_H__ 
