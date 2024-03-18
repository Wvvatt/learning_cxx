#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

template<typename T>
static const std::unordered_map<T, const char *> &enum_name_map();

template<typename T> 
static std::string to_string(T e)
{
    for (const auto& x : enum_name_map<T>())
    {
        if (x.first == e)
        {
            return x.second;
        }
    }
    return "";
}

template<typename T> 
static T to_enum(const std::string &name)
{
    for (const auto& i : enum_name_map<T>())
    {
        if (i.second == name)
        {
            return i.first;
        }
    }
    return static_cast<T>(0);
}

enum ProtType_e
{
    kUdp = 0,
    kTcp,
    kHttp
};

template<>
const std::unordered_map<ProtType_e, const char *> &enum_name_map()
{
	static std::unordered_map<ProtType_e, const char *> map = {
		{ProtType_e::kUdp,   "udp"},
        {ProtType_e::kTcp,   "tcp"},
        {ProtType_e::kHttp,  "http"}   
	};

	return map;
}

int main()
{
    std::cout << to_string(ProtType_e::kUdp) << std::endl;
    std::cout << to_enum<ProtType_e>("tcp") << std::endl;
    std::cout << to_enum<ProtType_e>("http") << std::endl;

    // 下面这条无法编译成功，提示Undefined reference to `std::vector<EnumNamePair<Size_e>, std::allocator<EnumNamePair<Size_e> > > const& enum_name_map<Size_e>()
    // std::cout << to_string(Size_e::small) << std::endl;     
}