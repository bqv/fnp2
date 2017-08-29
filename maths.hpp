#ifndef _MATHS_HPP_
#define _MATHS_HPP_

#include "list.hpp"

template<typename X> struct Even {
    static const bool value = (X::type::value % 2) == 0;
    typedef Bool<value> type;
};

template<typename X> struct Odd {
    static const bool value = (X::type::value % 2) == 1;
    typedef Bool<value> type;
};

template<typename L, typename R> struct IAdd {
    typedef Int<L::type::value + R::type::value> type;
};

template<typename L, typename R> struct ISub {
    typedef Int<L::type::value - R::type::value> type;
};

template<typename L, typename R> struct IMul {
    typedef Int<L::type::value * R::type::value> type;
};

template<typename L, typename R> struct IDiv {
    static_assert(R::type::value != 0, "Division by zero");
    typedef Int<L::type::value / R::type::value> type;
};

template<typename X, typename N> struct IPow {
    static_assert(N::type::value > 0, "Negative exponent");
    template<typename A, typename D, typename Y> struct F {
        template<typename B, typename I, class Enable = void> struct G {
            typedef typename F<B, typename ISub<I, Int<1>>::type, typename IMul<B, Y>::type>::type type;
        };
        template<typename B, typename I> struct G<B, I, typename std::enable_if<Even<I>::type::value>::type> {
            typedef typename G<typename IMul<B, B>::type, typename IDiv<I, Int<2>>::type>::type type;
        };
        typedef typename G<A, D>::type type;
    };
    template<typename A, typename Y> struct F<A, Int<0>, Y> {
        typedef typename Y::type type;
    };
    typedef typename F<X, typename ISub<N, Int<1>>::type, X>::type type;
};
template<typename X> struct IPow<X, Int<0>> {
    static const int value = 1;
    typedef Int<value> type;
};

template<int X, int Y> struct GCD {
    static const long value = GCD<Y, X % Y>::type::value;
    typedef Int<value> type;
};
template<int X> struct GCD<X, 0> {
    static const long value = X;
    typedef Int<value> type;
};

template<int N, typename R> struct Scale {
    typedef Rat<N*R::num, N*R::den> type;
};

template<typename R> struct Simplify {
    static const long gcd = GCD<R::num, R::den>::type::value;
    typedef Rat<R::num / gcd, R::den / gcd> type;
};

template<typename L, typename R> struct LCD {
    static const long gcd = GCD<L::den, R::den>::type::value;
    typedef typename Scale<R::den / gcd, L>::type Left;
    typedef typename Scale<L::den / gcd, R>::type Right;
};

template<typename L, typename R> struct RAdd {
    typedef LCD<L, R> lcd;
    static const long num = lcd::Left::num + lcd::Right::num;
    static const long den = lcd::Left::den;//lcd::Right::den;
    typedef typename Simplify<Rat<num, den>>::type type;
};

template<typename L, typename R> struct RSub {
    typedef LCD<L, R> lcd;
    static const long num = lcd::Left::num - lcd::Right::num;
    static const long den = lcd::Left::den;//lcd::Right::den;
    typedef typename Simplify<Rat<num, den>>::type type;
};

template<typename L, typename R> struct RMul {
    typedef LCD<L, R> lcd;
    static const long num = lcd::Left::num * lcd::Right::num;
    static const long den = lcd::Left::den * lcd::Right::den;
    typedef typename Simplify<Rat<num, den>>::type type;
};

template<typename L, typename R> struct RDiv {
    static_assert(R::num != 0, "Division by zero");
    typedef LCD<L, R> lcd;
    static const long num = lcd::Left::num * lcd::Right::den;
    static const long den = lcd::Left::den * lcd::Right::num;
    typedef typename Simplify<Rat<num, den>>::type type;
};

template<typename X> struct RExp {
    typedef List<Int<0>, List<Int<1>, List<Int<2>, List<Int<3>, List<Int<4>, List<Int<5>, List<Int<6>, List<Int<7>, List<Int<8> >>>>>>>>> Nats;
    template<typename N> using IAddOne = typename Curry<IAdd, Int<1>, N>::type;
    template<typename N> using RAddOne = typename Compose<Int2Rat, IAddOne>::template type<N>;
    template<typename N> using IPowX = typename Partial<IPow, X>::template type<N>;
    template<typename N> using RPowX = typename Compose<Int2Rat, IPowX>::template type<N>;
    typedef typename Map<RPowX, Nats>::type XS;
    typedef typename Scanl<RMul, Rat<1>, typename Map<RAddOne, Nats>::type>::type YS;
    typedef typename ZipWith<RDiv, XS, List<Rat<1>, YS>>::type AS;
    typedef typename Scanl<RAdd, Rat<0>, AS>::type BS;
    typedef typename Last<BS>::type type;
};

#endif /*MATHS_HPP*/
