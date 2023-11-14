#include <iomanip>
#include <iostream>
#include <type_traits>
 
#define IS_OBJECT(...) \
    std::cout << std::boolalpha << std::left << std::setw(9) << #__VA_ARGS__ \
              << (std::is_object<__VA_ARGS__>::value ? " is object\n" \
                                                : " is not an object\n")
 
int main()
{
    class cls {};
    enum class enm{

    };
 
    IS_OBJECT(void);
    IS_OBJECT(int);
    IS_OBJECT(int&);
    IS_OBJECT(int*);
    IS_OBJECT(int*&);
    IS_OBJECT(cls);
    IS_OBJECT(cls&);
    IS_OBJECT(cls*);
    IS_OBJECT(enm);
    IS_OBJECT(enm&);
    IS_OBJECT(enm*);
    IS_OBJECT(int());
    IS_OBJECT(int(*)());
    IS_OBJECT(int(&)());
}