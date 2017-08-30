#ifndef _FIZZBUZZ_HPP_
#define _FIZZBUZZ_HPP_

template<typename X, typename Y> struct Divides {
    typedef typename Eq<typename Mod<X, Y>::type, Int<0>>::type type;
};

template<typename N> struct Fizz {
    typedef typename If<typename Divides<N, Int<3>>::type, List<Char<'F'>, List<Char<'I'>, List<Char<'Z'>, List<Char<'Z'> >>>>, Nil>::type type;
};

template<typename N> struct Buzz {
    typedef typename If<typename Divides<N, Int<5>>::type, List<Char<'B'>, List<Char<'U'>, List<Char<'Z'>, List<Char<'Z'> >>>>, Nil>::type type;
};

template<typename N> struct FizzBuzz {
    typedef typename Concat<typename Fizz<N>::type, typename Buzz<N>::type>::type FB;
    typedef typename If<typename Eq<FB, Nil>::type, List<Char<' '>, List<N>>, List<Char<' '>, FB>>::type type;
};

template<typename L> struct Main {
    typedef typename Foldl<Concat, Nil, typename Map<FizzBuzz, L>::type>::type type;
};

#endif /*FIZZBUZZ_HPP*/
