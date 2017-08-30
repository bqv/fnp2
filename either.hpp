#ifndef _EITHER_HPP_
#define _EITHER_HPP_

template<template<typename> class F, const char* X> struct FMap<F, Left<X>> {
    typedef Left<X> type;
};
template<template<typename> class F, typename X> struct FMap<F, Right<X>> {
    typedef Right<typename F<X>::type> type;
};

template<const char* X, template<typename> class F> struct Bind<Left<X>, F> {
    typedef Left<X> type;
};
template<typename X, template<typename> class F> struct Bind<Right<X>, F> {
    typedef typename F<X>::type type;
};

#endif /*EITHER_HPP*/
