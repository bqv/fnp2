
#include <iostream>
#include "prelude.hpp"
#include "graph.hpp"
#include "fizzbuzz.hpp"

template<typename N> using PowTwo = typename Partial<IPow, Int<2>>::type<N>;
typedef typename Range<Int<1>, Int<100>>::type Nats;
typedef List<Char<'H'>, List<Char<'e'>, List<Char<'l'>, List<Char<'l'>, List<Char<'o'> >>>>> String;
typedef Apply<Template<Compose<SigmoidFunc,Int2Rat,PowTwo>::type>, Int<2>>::type SigmoidFour;
typedef typename RExp<Rat<1, 2>>::type E;

int main(int argc, char *argv[])
{
    std::cout << "Sqrt(E): " << E::value << " = " << E::num << "/" << E::den << std::endl;
    std::cout << "Sigmoid(4): " << SigmoidFour::value << std::endl;
    std::cout << "Length: " << Apply<Template<Length>, Nats>::type::value << std::endl;
    std::cout << "Second: " << Nth<Nats, 1>::type::value << std::endl;
    std::cout << "Index(3): " << Index<Nats, Int<3>>::type::value << std::endl;
    std::cout << "String: " << PutStrLn<String>();
    //std::cout << "FizzBuzz: " << PutStrLn<typename Main<Nats>::type>();
    return 0;
}

