#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_

template<long N, long D> using Weight = Rat<N, D>;
template<long N, long D> using Value = Rat<N, D>;

template<typename X> struct Sigmoid {
    typedef typename RDiv<Rat<1>, typename RAdd<Rat<1>, typename RExp<typename RNeg<X>::type>::type>::type>::type type;
};

#endif /*GRAPH_HPP*/
