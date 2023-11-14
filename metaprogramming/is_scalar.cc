#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <utility>
 
/*
    template<class T>
    struct is_scalar : std::integral_constant<bool, std::is_arithmetic<T>::value
                                                || std::is_enum<T>::value
                                                || std::is_pointer<T>::value
                                                || std::is_member_pointer<T>::value
                                                || std::is_null_pointer<T>::value>
    {};
*/
template<typename Head, typename... Tail>
void are_scalars(Head&& head, Tail&&... tail)
{
    using T = std::decay_t<decltype(head)>;
 
    std::cout << typeid(T).name() << " is "
              << (std::is_scalar_v<T> ? "" : "not ")
              << "a scalar\n";
 
    if constexpr (sizeof... (Tail))
    {
        are_scalars(std::forward<decltype(tail)>(tail)...);
    }
}
 
int main()
{
    bool flag = true;
    int i = 42;
    double d = 3.14;
    enum class E { e };
    int *p_i = &i;
    struct S { int m; } s;
    int S::* mp = &S::m;

    are_scalars(flag, i, d, E::e, p_i, "str", mp, nullptr, s);
}