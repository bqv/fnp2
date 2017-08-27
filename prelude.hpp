
template<bool B> struct Bool {
    static const bool value = B;
};

template<char C> struct Char {
    static const char value = C;
};

template<int N> struct Int {
    static const int value = N;
};

template<long N, long D = 1> struct Rat {
    static const long num = N;
    static const long den = D;
};

struct Nil {
    typedef Nil Head;
    typedef Nil Tail;
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

