#ifndef _LIST_HPP_
#define _LIST_HPP_

#include <type_traits>
#include "maths.hpp"

template<typename XS> struct Head {
    typedef typename XS::Head type;
};

template<typename XS> struct Tail {
    typedef typename XS::Tail type;
};

template<typename XS> struct Last {
    typedef typename XS::Tail Tail;
    typedef typename Last<Tail>::type type;
};
template<typename X> struct Last<List<X, Nil>> {
    typedef X type;
};

template<typename XS> struct Init {
    typedef typename XS::Head Head;
    typedef typename XS::Tail Tail;
    typedef List<Head, typename Init<Tail>::type> type;
};
template<typename X> struct Init<List<X, Nil>> {
    typedef Nil type;
};

template<typename XS> struct Length {
    typedef typename XS::Tail Tail;
    static const unsigned int tailLength = Length<Tail>::value;
    static const unsigned int value = 1 + tailLength;
    typedef Int<value> type;
};
template<> struct Length<Nil> {
    static const int value = 0;
    typedef Int<value> type;
};

template<typename XS, unsigned int N> struct Nth {
    typedef typename XS::Tail Tail;
    typedef typename Nth<Tail, N-1>::type type;
};
template<typename XS> struct Nth<XS, 0> {
    typedef typename XS::Head type;
};

template<class XS, class X> struct Member {
    typedef typename XS::Head Head;
    typedef typename XS::Tail Tail;
    static const bool value = Eq<X, Head>::value || Member<Tail, X>::value;
    typedef Bool<value> type;
};
template<class X> struct Member<Nil, X> {
    static const bool value = false;
    typedef Bool<value> type;
};

template<typename X, typename XS> struct Cons {
    typedef List<X, XS> type;
};

template<typename X, typename XS> struct Append {
    typedef typename XS::Head Head;
    typedef typename XS::Tail Tail;
    typedef typename Append<X, Tail>::type Rest;
    typedef List<Head, Rest> type;
};
template<typename X> struct Append<X, Nil> {
    typedef List<X, Nil> type;
};

template<typename XS, typename X> struct Index {
    template<typename YS, typename Y> struct Go {
        typedef typename YS::Head Head;
        typedef typename YS::Tail Tail;
        static const bool found = Eq<Y, Head>::type::value;
        static const int value = found ? 0 : 1 + Go<Tail, Y>::value;
    };
    template<typename Y> struct Go<Nil, Y> {
        static const int value = 0;
    };

    static_assert(Member<XS, X>::type::value, "Element not in list");
    static const int value = Go<XS, X>::value;
    typedef Int<value> type;
};

template<template<typename> class F, typename L> struct Map {
    typedef typename Apply<Template<F>, typename L::Head>::type X;
    typedef typename Map<F, typename L::Tail>::type XS;
    typedef List<X, XS> type;
};
template<template<typename> class F> struct Map<F, Nil> {
    typedef Nil type;
};

template<template<typename, typename> class F, typename L, typename R> struct ZipWith {
    typedef typename F<typename L::Head, typename R::Head>::type X;
    typedef typename ZipWith<F, typename L::Tail, typename R::Tail>::type XS;
    typedef List<X, XS> type;
};
template<template<typename, typename> class F, typename L> struct ZipWith<F, L, Nil> {
    typedef Nil type;
};
template<template<typename, typename> class F, typename R> struct ZipWith<F, Nil, R> {
    typedef Nil type;
};
template<template<typename, typename> class F> struct ZipWith<F, Nil, Nil> {
    typedef Nil type;
};

template<template<typename, typename> class F, typename A, typename L> struct Scanl {
    typedef typename F<A, typename L::Head>::type X;
    typedef typename Scanl<F, X, typename L::Tail>::type XS;
    typedef List<X, XS> type;
};
template<template<typename, typename> class F, typename A> struct Scanl<F, A, Nil> {
    typedef Nil type;
};

template<template<typename, typename> class F, typename A, typename L> struct Foldl {
    typedef typename Last<typename Scanl<F, A, L>::type>::type type;
};

#endif /*LIST_HPP*/
