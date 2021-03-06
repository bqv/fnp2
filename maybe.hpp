#ifndef _MAYBE_HPP_
#define _MAYBE_HPP_

#include "maths.hpp"

template<> struct Join<Nothing> {
    typedef Nothing type;
};
template<> struct Join<Just<Nothing>> {
    typedef Nothing type;
};
template<typename X> struct Join<Just<Just<X>>> {
    typedef Just<X> type;
};

template<template<typename> class F> struct FMap<F, Nothing> {
    typedef Nothing type;
};
template<template<typename> class F, typename X> struct FMap<F, Just<X>> {
    typedef Just<typename F<X>::type> type;
};

template<template<typename> class F> struct Bind<Nothing, F> {
    typedef Nothing type;
};
template<typename X, template<typename> class F> struct Bind<Just<X>, F> {
    typedef typename F<X>::type type;
};

template<template<typename,typename> class F> struct LiftM2<F, Nothing, Nothing> {
    typedef Nothing type;
};
template<template<typename,typename> class F, typename X> struct LiftM2<F, Just<X>, Nothing> {
    typedef Nothing type;
};
template<template<typename,typename> class F, typename Y> struct LiftM2<F, Nothing, Just<Y>> {
    typedef Nothing type;
};
template<template<typename,typename> class F, typename X, typename Y> struct LiftM2<F, Just<X>, Just<Y>> {
    typedef Just<typename F<X, Y>::type> type;
};

template<typename N, template<typename> class J, typename M> struct Maybe {
};
template<typename N, template<typename> class J> struct Maybe<N, J, Nothing> {
    typedef N type;
};
template<typename N, template<typename> class J, typename X> struct Maybe<N, J, Just<X>> {
    typedef typename J<X>::type type;
};

template<typename L> struct Sequence {
};
template<typename H, typename T> struct Sequence<List<Just<H>, T>> {
    template<typename R> using F = List<H, R>;
    typedef typename Bind<typename Sequence<T>::type, F>::type type;
};

template<typename XS> struct SafeHead {
    typedef Just<typename XS::Head> type;
};
template<> struct SafeHead<Nil> {
    typedef Nothing type;
};

template<typename XS> struct SafeTail {
    typedef Just<typename XS::Tail> type;
};
template<> struct SafeTail<Nil> {
    typedef Nothing type;
};

template<typename XS> struct SafeLast {
    template<typename YS> struct Go {
        typedef typename YS::Tail Tail;
        typedef typename Go<Tail>::type type;
    };
    template<typename Y> struct Go<List<Y, Nil>> {
        typedef Y type;
    };
    typedef Just<typename Go<XS>::type> type;
};
template<> struct SafeLast<Nil> {
    typedef Nothing type;
};

template<typename XS> struct SafeInit {
    template<typename YS> struct Go {
        typedef typename YS::Head Head;
        typedef typename YS::Tail Tail;
        typedef List<Head, typename Go<Tail>::type> type;
    };
    template<typename Y> struct Go<List<Y, Nil>> {
        typedef Nil type;
    };
};
template<> struct SafeInit<Nil> {
    typedef Nothing type;
};

template<template<typename, typename> class F, typename XS, typename YS> struct SafeZipWith {
    typedef typename F<typename XS::Head, typename YS::Head>::type Z;
    typedef typename SafeZipWith<F, typename XS::Tail, typename YS::Tail>::type Rest;
    template<typename ZS> using ConsZ = typename Partial<List, Z>::template type<ZS>;
    typedef typename FMap<ConsZ, Rest>::type type;
};
template<template<typename, typename> class F, typename XS> struct SafeZipWith<F, XS, Nil> {
    typedef Nothing type;
};
template<template<typename, typename> class F, typename YS> struct SafeZipWith<F, Nil, YS> {
    typedef Nothing type;
};
template<template<typename, typename> class F> struct SafeZipWith<F, Nil, Nil> {
    typedef Just<Rat<0>> type;
};

template<typename XS, typename YS> struct SafeDot {
    typedef typename RSum<typename SafeZipWith<RMul, XS, YS>::type>::type type;
};

#endif /*MAYBE_HPP*/
