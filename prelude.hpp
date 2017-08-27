
template<bool B> struct Bool {
    typedef Bool<B> type;
    static const bool value = B;
};

template<char C> struct Char {
    typedef Char<C> type;
    static const char value = C;
};

template<int N> struct Int {
    typedef Int<N> type;
    static const int value = N;
};

template<long N, long D = 1> struct Rat {
    typedef Rat<N, D> type;
    static const long num = N;
    static const long den = D;
};

struct Nil {
};

template<typename H, typename T=Nil> struct List {
    typedef H Head;
    typedef T Tail;
};

template<typename... XS> struct Array {
};

template<template<typename> class> struct Template {
};

template<typename X> struct Void {
    typedef void type;
};

template<typename X, typename Y = void> struct Eval {
    typedef X type;
};
template<typename X> struct Eval<X, typename Void<typename X::type>::type> {
    typedef typename Eval<typename X::type>::type type;
};

template<typename F, typename X> struct Apply {
    typedef typename Apply<Eval<F>, X>::type type;
};
template<template<typename> class F, typename X> struct Apply<Template<F>, X> {
    typedef typename F<X>::type type;
};

template<template<typename> class F, template<typename> class G> struct Compose {
    template<typename X> using type = F<G<X>>;
};

template<template<typename...> class F, typename... XS> struct Curry {
    using type = F<XS...>;
};
template<template<typename...> class F> struct Curry<F> {
    using type = F<>;
};

template<typename X> struct Id {
    typedef X type;
};

template<class X, class Y> struct Eq {
    static const bool value = false;
};
template<class X> struct Eq<X,X> {
    static const bool value = true;
};

#include "maths.hpp"
#include "list.hpp"

