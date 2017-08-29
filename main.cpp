
#include <iostream>
#include "prelude.hpp"

template<long N, long D> using Weight = Rat<N, D>;
template<long N, long D> using Value = Rat<N, D>;

template<typename N> using AddOne = typename Curry<IAdd, Int<1>, N>::type;
template<typename N> using PowTwo = typename Partial<IPow, Int<2>>::type<N>;
typedef List<Int<0>, List<Int<1>, List<Int<2>, List<Int<3> >>>> Nats;

typedef typename RExp<Int<1>>::type E;

int main(int argc, char *argv[])
{
    std::cout << "E: " << 1.0 * E::num / E::den << " = " << E::num << "/" << E::den << std::endl;
    std::cout << "Sixteen: " << Apply<Template<Compose<PowTwo,PowTwo>::type>, Int<2>>::type::value << std::endl;
    std::cout << "First: " << Id<Head<Map<AddOne, Nats>::type>::type>::type::value << std::endl;
    std::cout << "Length: " << Apply<Template<Length>, Nats>::type::value << std::endl;
    std::cout << "Second: " << Nth<Nats, 1>::type::value << std::endl;
    std::cout << "Index3: " << Index<Nats, Int<3>>::type::value << std::endl;
    return 0;
}

// TODO: \x -> take 10 . scanl (+) 0 $ zipWith (/) (map (x^^) [0..]) (scanl (*) 1 [1..])

