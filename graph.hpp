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

template<typename I, typename O, typename G, typename B, typename N> struct RunLayer {
    template<typename X> using GetValues = typename GetValue<X, G>::type;
    static constexpr char mvif[] = "Missing value in feedforward";
    typedef typename Maybe<Left<mvif>, Right, typename Sequence<typename Map<GetValues, I>::type>::type>::type Values;
    template<typename X> using GetLinks = typename LinksFrom<X, G>::type;
    typedef typename Map<GetLinks, I>::type Links;
    template<typename X, typename YS> struct FindWeights {
        template<typename Y> using GetWeights = typename GetWeight<X, Y, G>::type;
        typename Sequence<typename Map<GetWeights, YS>::type>::type type;
    };
    static constexpr char mgdf[] = "Mismatched graph during feedforward";
    typedef typename Maybe<Left<mgdf>, Right, typename SafeZipWith<FindWeights, I, Links>::type>::type Weights;
};

template<typename X, typename G, typename N> struct RunNetworkImpl {
    typedef Right<G> type;
};
template<typename X, typename Y, typename YS, typename G, typename N> struct RunNetworkImpl<List<X, List<Y, YS>>, G, N> {
    template<typename Z> using GetBiases = typename FindWithDefault<Z, Int<0>, typename N::Bias>::type;
    typedef typename Map<GetBiases, Y>::type Biases;
    typedef typename RunLayer<X, Y, G, Biases, typename N::Nonlin>::type LayerValues;
    template<typename Z> using GoNext = typename RunNetworkImpl<List<Y, YS>, Z, N>::type;
    typedef typename Bind<LayerValues, GoNext>::type type;
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
    template<typename X> using FoldWithGraph = typename Foldl<BiApply, typename N::Graph, X>::type;
    typedef typename FMap<FoldWithGraph, Insertions>::type InputGraph;
    template<typename X> using MakeGraph = typename RunNetworkImpl<typename N::Layers, X, N>::type;
    typedef typename Bind<InputGraph, MakeGraph>::type NewGraph;
    template<typename X> using UpdateGraph = typename UpdateNetwork<N, X>::type;
    typedef typename FMap<UpdateGraph, NewGraph>::type type;
};

#endif /*GRAPH_HPP*/
