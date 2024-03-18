#include "test.h"

#include <cassert>
#include <iostream>

int main(int /*argc*/, char * /*argv*/[])
{
	// // Convert from a Master enumerator to a string
    // std::string name;
	// // Convert from a string to a Master enumerator
    // Master master = Tigress;
    // const bool bResult = EnumString<Master>::ToEnum( master, masterStr );
    // assert( bResult == true );
    // assert( master == Monkey );

    std::cout << EnumString<Master>::ToString(Mantis) << std::endl;
    std::cout << EnumString<Master>::ToEnum("viper") << std::endl;
    std::cout << EnumString<Master>::ToEnum("crane") << std::endl;
    std::cout << EnumString<Master>::ToEnum("watt") << std::endl;

    return 0;
}
