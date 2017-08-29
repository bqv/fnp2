#ifndef _ASSOC_HPP_
#define _ASSOC_HPP_

template<typename X> struct Fst {
    typedef typename X::Fst type;
};

template<typename X> struct Snd {
    typedef typename X::Snd type;
};

template<typename X, typename L> struct Lookup {
    typedef typename L::Head Head;
    typedef typename L::Tail Tail;
    typedef Just<typename If<Eq<X, typename Head::Fst>, Just<typename Head::Snd>, typename Lookup<X, Tail>::type>::type> type;
};
template<typename X> struct Lookup<X, Nil> {
    typedef Nothing type;
};

template<typename K, typename V, typename L> struct FindWithDefault {
    typedef typename Lookup<K, L>::type Result;
    typedef typename If<typename Eq<Nothing, Result>::type, V, typename Result::FromJust>::type type;
};

template<typename L> struct Keys {
    typedef typename Map<Fst, L>::type type;
};

template<typename K, typename L> struct IndexKey {
    template<typename X, typename XS> struct Go {
        typedef typename XS::Head Head;
        typedef typename XS::Tail Tail;
        static const bool found = Eq<K, typename Head::Fst>::type::value;
        static const int value = found ? 0 : 1 + Go<X, Tail>::type::value;
        typedef Int<value> type;
    };
    template<typename X> struct Go<X, Nil> {
        static const int value = 0;
        typedef Int<value> type;
    };

    typedef typename If<typename Member<typename Keys<L>::type, K>::type, Just<typename Go<K, L>::type>, Nothing>::type type;
};

template<typename K, typename L> struct Delete {
    typedef typename L::Head Head;
    typedef typename L::Tail Tail;
    typedef typename Delete<K, Tail>::type Rest;
    typedef typename If<typename Eq<K, typename Head::Fst>::type, Rest, List<Head, Rest>>::type type;
};

template<typename K, typename V, typename L> struct Insert {
    typedef List<Tuple<K, V>, typename Delete<K, L>::type> type;
};

#endif /*ASSOC_HPP*/
