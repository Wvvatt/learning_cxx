#include "EnumString.h"

#include <iostream>

enum Master
{
    Tigress = 0,
    Viper,
    Monkey,
    Mantis,
    Crane
};

// String support for Furious Five Master
Begin_Enum_String(Master)
{
    Enum_String(Tigress, "tigress");
    Enum_String(Viper, "viper");
    Enum_String(Monkey, "monkey");
    Enum_String(Mantis, "mantis");
    Enum_String(Crane, "crane");
}
End_Enum_String;