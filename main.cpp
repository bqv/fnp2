
#include <iostream>
#include "prelude.hpp"

template<long N, long D> using Weight = Rat<N, D>;
template<long N, long D> using Value = Rat<N, D>;

template<typename N> using PowTwo = typename Partial<IPow, Int<2>>::type<N>;
typedef List<Int<0>, List<Int<1>, List<Int<2>, List<Int<3> >>>> Nats;

typedef typename RExp<Rat<1, 2>>::type E;

int main(int argc, char *argv[])
{
    std::cout << "sqrt E: " << 1.0 * E::num / E::den << " = " << E::num << "/" << E::den << std::endl;
    std::cout << "Sixteen: " << Apply<Template<Compose<PowTwo,PowTwo>::type>, Int<2>>::type::value << std::endl;
    std::cout << "Length: " << Apply<Template<Length>, Nats>::type::value << std::endl;
    std::cout << "Second: " << Nth<Nats, 1>::type::value << std::endl;
    std::cout << "Index3: " << Index<Nats, Int<3>>::type::value << std::endl;
    return 0;
}

