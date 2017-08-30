#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_

// Types

template<long N, long D = 1> using Weight = Rat<N, D>;
template<long N, long D = 1> using Value = Rat<N, D>;

// Nonlinearities

template<template<typename> class F, template<typename> class D> struct NonLin {
    template<typename X> using Func = typename F<X>::type;
    template<typename X> using Deriv = typename D<X>::type;
};

template<typename X> struct SigmoidFunc {
    typedef typename RDiv<Rat<1>, typename RAdd<Rat<1>, typename RExp<typename RNeg<X>::type>::type>::type>::type type;
};

template<typename X> struct SigmoidDeriv {
    typedef typename RMul<X, typename RSub<Rat<1>, X>::type>::type type;
};

typedef NonLin<SigmoidFunc, SigmoidDeriv> Sigmoid;

template<typename X> struct StepFunc {
    typedef typename If<Bool<X::type::value >= 0>, Int<1>, Int<-1>>::type type;
};

template<typename X> struct StepDeriv {
    typedef Int<0> type;
};

typedef NonLin<StepFunc, StepDeriv> Step;

// Graphs

template<typename F, typename T, typename A> struct GetWeight {
    typedef typename Lookup<Tuple<F,T>, typename A::Fst>::type type;
};

template<typename N, typename A> struct GetValue {
    typedef typename Lookup<N, typename A::Snd>::type type;
};

template<typename F, typename T, typename W, typename A> struct SetWeight {
    typedef typename Insert<Tuple<F,T>, W, typename A::Fst>::type graph;
    typedef Tuple<graph, typename A::Snd> type;
};

template<typename N, typename V, typename A> struct SetValue {
    typedef typename Insert<N, V, typename A::Snd>::type nodes;
    typedef Tuple<typename A::Fst, nodes> type;
};

template<typename N, typename A> struct LinksFrom {
    template<typename K> using Filter = typename Eq<N, typename K::Fst>::type;
    typedef typename Map<Snd, typename Keys<typename FilterKeys<Filter, typename A::Fst>::type>::type>::type type;
};

// Networks

template<typename G, typename L, typename N, typename A, typename B> struct Network {
    typedef G Graph;
    typedef L Layers;
    typedef N Nonlin;
    typedef A Alpha;
    typedef B Bias;
};

template<typename N, typename G> struct UpdateNetwork {
    typedef Network<G, typename N::Layers, typename N::Nonlin, typename N::Alpha, typename N::Bias> type;
};

template<typename N> struct GetOutput {
    static constexpr char nlig[] = "No layers in graph";
    typedef typename Maybe<Left<nlig>, Right, typename SafeLast<typename N::Layers>::type>::type A;
    template<typename X> using F = typename GetValue<X, typename N::Graph>::type;
    static constexpr char mvio[] = "Missing value in output";
    template<typename XS> using B = typename Maybe<Left<mvio>, Right, typename Sequence<typename Map<F, XS>::type>::type>::type;
    typedef typename Bind<A, B>::type type;
};

template<typename N, typename I> struct RunNetwork {
    template<typename X> using FindOrZero = typename FindWithDefault<X, Int<0>, typename N::Bias>::type;
    template<typename XS> using GetBiases = typename Map<FindOrZero, XS>::type;
    typedef typename Bind<typename SafeHead<typename N::Layers>::type, GetBiases>::type Biases;
    template<typename X> struct CurriedAdd {
        template<typename Y> using Add = typename IAdd<X, Y>::type;
        typedef Template<Add> type;
    };
    template<typename X, typename Y> using BiApply = typename Apply<X, Y>::type;
    template<typename XS> using GetInputs = typename SafeZipWith<BiApply, typename Map<CurriedAdd, I>::type, XS>::type;
    typedef typename Bind<Biases, GetInputs>::type Inputs;
    template<typename X, typename Y> struct CurriedSetValue {
        template<typename Z> using SetVal = typename SetValue<X, Y, Z>::type;
        typedef Template<SetVal> type;
    };
    template<typename XS, typename YS> using SetValues = typename SafeZipWith<CurriedSetValue, XS, YS>::type;
    static constexpr char eii[] = "Error insesrting input";
    typedef typename Maybe<Left<eii>, Right, typename Join<typename LiftM2<SetValues, typename N::Layers, Inputs>::type>::type>::type Insertions;
};

#endif /*GRAPH_HPP*/
