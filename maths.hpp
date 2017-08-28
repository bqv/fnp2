
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
/**
 * pow :: Int -> Int -> Int
 * pow x 0           =  1
 * pow x n | n > 0   =  f x (n-1) x where
 *   f a 0 acc = acc
 *   f a d acc = g a d  where
 *     g b i | even i  = g (b*b) (i `quot` 2)
 *           | otherwise = f b (i-1) (b*acc)
 * pow x n           = error "Prelude.^: negative exponent"
 */
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
    static const long result = GCD<Y, X % Y>::value;
};
template<int X> struct GCD<X, 0> {
    static const long value = X;
};

template<int N, typename R> struct Scale {
    typedef Rat<N*R::num, N*R::den> type;
};

template<typename R> struct Simplify {
    static const long gcd = GCD<R::num, R::den>::value;
    typedef Rat<R::num / gcd, R::den / gcd> type;
};

template<typename L, typename R> struct LCD {
    static const long gcd = GCD<L::den, R::den>::value;
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
    static_assert(R::num::value != 0, "Division by zero");
    typedef LCD<L, R> lcd;
    static const long num = lcd::Left::num * lcd::Right::dec;
    static const long den = lcd::Left::den * lcd::Right::num;
    typedef typename Simplify<Rat<num, den>>::type type;
};

