#include <iostream>
#include <type_traits>
#include <memory>
#include <vector>
#include <sstream>

template <typename T>
std::unique_ptr<T> WrapUnique(T *ptr)
{
  static_assert(!std::is_array<T>::value, "array types are unsupported");
  static_assert(std::is_object<T>::value, "non-object types are unsupported");
  return std::unique_ptr<T>(ptr);
}

struct MemberParameter
{
  const char *key;
  void *member_ptr;
  void (*parse)(const std::string &src, void *target);
  void (*encode)(const void *src, std::string *target);
};

template <typename T, typename = void>
struct TypedParser
{
  static void Parse(const std::string &src, void *target)
  {
    T *obj = static_cast<T *>(target);
    auto str = src.substr(1, src.size() - 2);
    obj->Parse(str);
  }
  static void Encode(const void *src, std::string *target)
  {
    T *obj = const_cast<T *>(static_cast<const T *>(src));
    target->append("{").append(obj->Encode()).append("}");
  }
};

template <typename T>
static void AddMembers(MemberParameter *out, const char *key, T *member)
{
  *out = MemberParameter{
      key, member,
      &TypedParser<T>::Parse,
      &TypedParser<T>::Encode}; // add member时根据类型添加不同的parse和encode方法
}

template <typename T, typename... Args>
static void AddMembers(MemberParameter *out,
                       const char *key,
                       T *member,
                       Args... args)
{
  AddMembers(out, key, member);
  AddMembers(++out, args...); // 递归调用，自动匹配完成循环
}

std::vector<std::string> split(const std::string &str, char delimiter)
{
  std::vector<std::string> tokens;
  std::stringstream ss(str);
  std::string token;

  while (std::getline(ss, token, delimiter))
  {
    tokens.push_back(token);
  }

  return tokens;
}

std::vector<std::string> split_kv(const std::string &str, char delimiter)
{
  std::vector<std::string> tokens;

  auto offset = str.find_first_of(delimiter);
  tokens.emplace_back(str.substr(0, offset));
  tokens.emplace_back(str.substr(offset + 1));

  return tokens;
}

class ObjectParser
{
public:
  template <typename T, typename... Args>
  static std::unique_ptr<ObjectParser> Create(const char *first_key,
                                                        T *first_member,
                                                        Args... args)
  {
    std::vector<MemberParameter> members(sizeof...(args) / 2 + 1);
    AddMembers(&members.front(), std::move(first_key), first_member, args...);
    return WrapUnique(new ObjectParser(std::move(members)));
  }

  void Parse(const std::string &src)
  {
    auto arr = split(src, ',');
    for (auto &&s : arr)
    {
      auto pair = split_kv(s, ':');
      for (auto &m : members_)
      {
        if (m.key == pair[0])
        {
          m.parse(pair[1], m.member_ptr);
        }
      }
    }
  }
  std::string Encode() const
  {
    std::string res;
    bool first = true;
    for (const auto &member : members_)
    {
      if (!first)
        res += ",";
      res += member.key;
      res += ":";
      member.encode(member.member_ptr, &res);
      first = false;
    }
    return res;
  };

private:
  explicit ObjectParser(
      std::vector<MemberParameter> members) : members_(std::move(members)){};

  std::vector<MemberParameter> members_;
};

template <>
struct TypedParser<bool>
{
  static void Parse(const std::string &src, void *target)
  {
    if (src == "true")
      *static_cast<bool *>(target) = true;
    else
      *static_cast<bool *>(target) = false;
  }
  static void Encode(const void *src, std::string *target)
  {
    const auto *ssrc = reinterpret_cast<const bool *>(src);
    if (*ssrc)
      target->append("true");
    else
      target->append("false");
  }
};

template <>
struct TypedParser<std::string>
{
  static void Parse(const std::string &src, void *target)
  {
    *static_cast<std::string *>(target) = static_cast<std::string>(src);
  }
  static void Encode(const void *src, std::string *target)
  {
    target->append(*static_cast<const std::string *>(src));
  }
};

template <typename T>
struct TypedParser<T, typename std::enable_if<std::is_same<T, std::vector<typename T::value_type>>::value>::type>
{
  static void Parse(const std::string &src, void *target)
  {
    T *arr = static_cast<T *>(target);
    arr->clear();
    std::vector<std::string> str_arr = split(src.substr(1, src.size() - 2), '/');
    for(const auto &str : str_arr){
      arr->emplace_back();
      TypedParser<typename T::value_type>::Parse(str, &arr->back());
    }
  }
  static void Encode(const void *src, std::string *target)
  {
    const T *arr = static_cast<const T *>(src);
    target->append("[");
    for(const auto &obj : *arr){
      TypedParser<typename T::value_type>::Encode(static_cast<const void *>(&obj), target);
      target->append("/");
    }
    target->erase(target->size() - 1);
    target->append("]");
  }
};

// template <typename T>
// struct TypedParser<T, typename std::enable_if<std::is_class<T>::value>::type> 
// {
//   static void Parse(const std::string &src, void *target)
//   {
//     T *obj = static_cast<T *>(target);
//     auto str = src.substr(1, src.size() - 2);
//     obj->Parser()->Parse(str);
//   }
//   static void Encode(const void *src, std::string *target)
//   {
//     T *obj = const_cast<T *>(static_cast<const T *>(src));
//     target->append("{").append(obj->Parser()->Encode()).append("}");
//   }
// };

template <typename T>
struct TypedParser<T, typename std::enable_if<std::is_integral<T>::value>::type> 
{
  static void Parse(const std::string &src, void *target)
  {
    *static_cast<T *>(target) = static_cast<T>(std::stoi(src));
  }
  static void Encode(const void *src, std::string *target)
  {
    const auto *ssrc = reinterpret_cast<const T *>(src);
    target->append(std::to_string(*ssrc));
  }
};

struct Obj1
{
  std::string name;

  std::unique_ptr<ObjectParser> Parser()
  {
    return ObjectParser::Create(
        "name", &name);
  }
};

struct Obj2
{
  bool flag = true;
  int number = 100;
  Obj1 obj1{};
  std::vector<std::string> arr{};

  std::unique_ptr<ObjectParser> Parser()
  {
    return ObjectParser::Create(
        "flag", &flag,
        "number", &number,
        "obj1", obj1.Parser().release(),
        "arr", &arr);
  }
};

int main() 
{
    Obj2 obj;
    obj.Parser()->Parse(R"(flag:true,number:0,obj1:{name:obj_watt},arr:[one/two/three])");
    std::cout << obj.Parser()->Encode() << std::endl;
    return 0;
}