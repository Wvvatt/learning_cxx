#include "enumstring.h"

#include <iostream>

int main()
{
    std::cout << EnumString<ConnMode_e>::From(kListener) << std::endl;
}