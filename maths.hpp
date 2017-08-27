
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

