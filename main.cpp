
#include <iostream>
#include "prelude.hpp"

template<long N, long D> using Weight = Rat<N, D>;
template<long N, long D> using Value = Rat<N, D>;

int main(int argc, char *argv[])
{
    typedef List<Int<1>,
            List<Int<2>,
            List<Int<3>
                >>> Lst;
    std::cout << "First: " << Head<Map<Id, Lst>>::type::value << std::endl;
    std::cout << "Length: " << Apply<Template<Length>, Lst>::type::value << std::endl;
    std::cout << "Second: " << Nth<Lst, 1>::type::value << std::endl;
    std::cout << "Index3: " << Index<Lst, Int<3>>::type::value << std::endl;
    return 0;
}

// \x -> take 10 . scanl (+) 0 $ zipWith (/) (map (x^^) [0..]) (scanl (*) 1 [1..])

