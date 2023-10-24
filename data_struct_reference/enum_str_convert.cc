#include <vector>
#include <string>
#include <iostream>

template <typename T>
struct EnumNamePair
{
	T data;
	const char *name;
};

template<typename T>
const std::vector<EnumNamePair<T>> &enum_name_map();

template<typename T> std::string to_string(T data)
{
    for (const auto& x : enum_name_map<T>())
    {
        if (x.data == data)
        {
            return x.name;
        }
    }
    return "";
}

template<typename T> T to_enum(const std::string &name)
{
    for (const auto& i : enum_name_map<T>())
    {
        if (i.name == name)
        {
            return i.data;
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
const std::vector<EnumNamePair<ProtType_e>> &enum_name_map()
{
	static std::vector<EnumNamePair<ProtType_e>> map = {
		{ProtType_e::kUdp,   "udp"},
        {ProtType_e::kTcp,   "tcp"},
        {ProtType_e::kHttp,  "http"}   
	};

	return map;
}

enum Size_e
{
    small = 0,
    middle,
    big
};

int main()
{
    std::cout << to_string(ProtType_e::kUdp) << std::endl;
    std::cout << to_enum<ProtType_e>("tcp") << std::endl;

    // 下面这条无法编译成功，提示Undefined reference to `std::vector<EnumNamePair<Size_e>, std::allocator<EnumNamePair<Size_e> > > const& enum_name_map<Size_e>()
    // std::cout << to_string(Size_e::small) << std::endl;     
}