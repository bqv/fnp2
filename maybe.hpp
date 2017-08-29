#ifndef _MAYBE_HPP_
#define _MAYBE_HPP_

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

#endif /*MAYBE_HPP*/
