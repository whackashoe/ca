#include <type_traits>
#include <iostream>

template<typename, typename T>
struct has_serialize { };

// specialization that does the checking

template<typename C, typename Ret, typename... Args>
struct has_serialize<C, Ret(Args...)> {
private:
    template<typename T>
    static constexpr auto check(T*)
    -> typename
        std::is_same<
            decltype( std::declval<T>().serialize( std::declval<Args>()... ) ),
            Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        >::type;  // attempt to call it and see if the return type is correct

    template<typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};

struct X {
    int serialize(const std::string&) { return 42; } 
};

int main()
{
    auto a = 1;

    template<typename, typename T>
    struct has_m { };

    // specialization that does the checking

    template<typename C, typename Ret, typename... Args>
    struct has_m<C, Ret(Args...)> {
    private:
        template<typename T>
        static constexpr auto check(T*)
        -> typename
            std::is_same<
                decltype( std::declval<T>().serialize( std::declval<Args>()... ) ),
                Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            >::type;  // attempt to call it and see if the return type is correct

        template<typename>
        static constexpr std::false_type check(...);

        typedef decltype(check<C>(0)) type;

    public:
        static constexpr bool value = type::value;
    };
    //has_method<X, int(const std::string&)>("serialize");
	std::cout << has_serialize<X,  int(const std::string&)>::value; // will print 1
}