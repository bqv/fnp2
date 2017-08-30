#ifndef _PRELUDE_HPP_
#define _PRELUDE_HPP_

#include <sstream>

template<bool B> struct Bool {
    typedef Bool<B> type;
    static const bool value = B;
};

template<char C> struct Char {
    typedef Char<C> type;
    static const char value = C;
};

template<long N> struct Int {
    typedef Int<N> type;
    static const long value = N;
  public:
    Int() {}
    static Int<N> New() {
        return Int();
    }
    template<long I> friend std::ostream& operator<<(std::ostream& os, const Int<I>& s);
};

template<long N> std::ostream& operator<<(std::ostream& os, const Int<N>& s) {
    os << N;
    return os;
};

template<typename... DS> struct Integer {
};

template<long N, long D = 1> struct Rat {
    typedef Rat<N, D> type;
    static const long long num = N;
    static const long long den = D;
    static constexpr long double value = 1.0 * num / den;
};

template<typename X> struct Int2Rat {
    typedef Rat<X::type::value> type;
};

struct Nil {
};
template<typename H, typename T=Nil> struct List {
    typedef H Head;
    typedef T Tail;
};
template<char C, typename T> class List<Char<C>, T> {
  public:
    typedef Char<C> Head;
    typedef T Tail;
    List() {}
    static List<Char<C>, T> New() {
        return List();
    }
    template<char D, typename S> friend std::ostream& operator<<(std::ostream& os, const List<Char<D>, S>& s);
    template<char D, typename S> friend std::ostream& operator<<(std::ostream& os, const List<Int<D>, S>& s);
};

template<char C, typename T> std::ostream& operator<<(std::ostream& os, const List<Char<C>, T>& s) {
    os << C << typename T::List();
    return os;
};
template<char C> std::ostream& operator<<(std::ostream& os, const List<Char<C>, Nil>& s) {
    os << C;
    return os;
};
template<long N, typename T> std::ostream& operator<<(std::ostream& os, const List<Int<N>, T>& s) {
    os << N << ' ' << typename T::List();
    return os;
};
template<long N> std::ostream& operator<<(std::ostream& os, const List<Int<N>, Nil>& s) {
    os << N << ' ';
    return os;
};
std::ostream& operator<<(std::ostream& os, const Nil& s) {
    return os;
};

template<typename X> std::string PutStrLn() {
    std::ostringstream os;
    os << X::New() << std::endl;
    return os.str();
};

template<typename... XS> struct Array {
};

template<typename X, typename Y> struct Tuple {
    typedef X Fst;
    typedef Y Snd;
};

struct Nothing {
    template<template<typename> class F> struct Bind {
        typedef Nothing type;
    };
};
template<typename X> struct Just {
    typedef X FromJust;
    template<template<typename> class F> struct Bind {
        typedef typename F<X>::type type;
    };
};

template<const char* X> struct Left {
    const char* FromLeft = X;
};
template<typename X> struct Right {
    typedef X FromRight;
};

template<typename C, typename T, typename F> struct If {
};
template<typename T, typename F> struct If<Bool<true>, T, F> {
    typedef T type;
};
template<typename T, typename F> struct If<Bool<false>, T, F> {
    typedef F type;
};

template<template<typename...> class F, typename...> struct Template {
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

template<template<typename> class F, template<typename> class G, template<typename> class... Rest> struct Compose {
    template<typename X> using templ = typename Compose<F, G>::template type<X>;
    template<typename X> using type = typename Compose<templ, Rest...>::template type<X>;
};
template<template<typename> class F, template<typename> class G> struct Compose<F, G> {
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

template<template<typename,typename,typename...> class F> struct Flip {
    template<typename X, typename Y, typename... ZS> using type = typename F<Y, X, ZS...>::type;
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

template<typename M> struct Join {
    static_assert(Not<typename Eq<M, M>::type>::type::value, "No instance for type");
};

template<template<typename> class F, typename M> struct FMap {
    static_assert(Not<typename Eq<M, M>::type>::type::value, "No instance for type");
};

template<typename M, template<typename> class F> struct Bind {
    static_assert(Not<typename Eq<M, M>::type>::type::value, "No instance for type");
};

template<template<typename,typename> class F, typename M1, typename M2> struct LiftM2 {
    static_assert(Not<typename Eq<M1, M1>::type>::type::value, "No instance for type");
};

#include "maybe.hpp"
#include "either.hpp"
#include "maths.hpp"
#include "list.hpp"
#include "assoc.hpp"

#endif /*PRELUDE_HPP*/
