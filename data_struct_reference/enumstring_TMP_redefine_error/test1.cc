#include "enumstring.h"
#include <iostream>

void Test1()
{
    std::cout << EnumString<ConnMode_e>::From(kListener) << std::endl;
}