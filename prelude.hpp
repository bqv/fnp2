
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

template<template<typename...> class, typename...> struct Template {
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

template<typename T, typename = void> struct DoApply {
    typedef T type;
};
template<template<typename...> class F, typename... XS> struct DoApply<Template<F, XS...>, typename Void<typename F<XS...>::type>::type> {
    typedef typename F<XS...>::type type;
};

template<typename F, typename... XS> struct Apply {
    typedef typename Apply<Eval<F>, XS...>::type type;
};
template<template<typename...> class F, typename... XS, typename T1, typename T2, typename... TS> struct Apply<Template<F, XS...>, T1, T2, TS...> {
    typedef typename Apply<typename DoApply<Template<F, XS..., T1>>::type, T2, TS...>::type type;
};
template<template<typename...> class F, typename... XS, typename T> struct Apply<Template<F, XS...>, T> {
    typedef typename DoApply<Template<F, XS..., T>>::type type;
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

template<template<typename...> class F, typename... XS> struct Partial {
    template<typename... YS> using type = typename Curry<F, XS..., YS...>::type;
};

template<typename X> struct Id {
    typedef X type;
};

template<typename X> struct Not {
    static const bool value = !(X::type::value);
    typedef Bool<value> type;
};

template<class X, class Y> struct Eq {
    static const bool value = false;
    typedef Bool<value> type;
};
template<class X> struct Eq<X,X> {
    static const bool value = true;
    typedef Bool<value> type;
};

#include "maths.hpp"
#include "list.hpp"

