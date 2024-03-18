#include "test.h"

void Test2()
{
    std::cout << EnumString<Master>::ToString(Mantis) << std::endl;
    std::cout << EnumString<Master>::ToEnum("viper") << std::endl;
    std::cout << EnumString<Master>::ToEnum("crane") << std::endl;
    std::cout << EnumString<Master>::ToEnum("watt") << std::endl;
}