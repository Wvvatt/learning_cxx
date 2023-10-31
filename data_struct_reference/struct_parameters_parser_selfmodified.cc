/*
 *  From WebRTC
 */
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>

/*
  From Abseil library
*/
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

template <typename T>
struct TypedParser
{
  static void Parse(const std::string &src, void *target)
  {
    *reinterpret_cast<T *>(target) = static_cast<T>(src);
  }
  static void Encode(const void *src, std::string *target)
  {
    target->append(*static_cast<const T *>(src));
  }
};

template <>
struct TypedParser<bool>
{
  static void Parse(const std::string &src, void *target)
  {
    if (src == "true")
    {
      *reinterpret_cast<bool *>(target) = true;
    }
    else
    {
      *reinterpret_cast<bool *>(target) = false;
    }
  }
  static void Encode(const void *src, std::string *target)
  {
    const auto *ssrc = reinterpret_cast<const bool *>(src);
    if (*ssrc)
    {
      target->append("true");
    }
    else
    {
      target->append("false");
    }
  }
};

template <>
struct TypedParser<int>
{
  static void Parse(const std::string &src, void *target)
  {
    *reinterpret_cast<int *>(target) = std::stoi(src);
  }
  static void Encode(const void *src, std::string *target)
  {
    const auto *ssrc = reinterpret_cast<const int *>(src);
    target->append(std::to_string(*ssrc));
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

struct SelfDefineObj
{
  std::string name;

  std::unique_ptr<ObjectParser> Parser()
  {
    return ObjectParser::Create(
        "name", &name);
  }
};

template <>
struct TypedParser<SelfDefineObj>
{
  static void Parse(const std::string &src, void *target)
  {
    SelfDefineObj *obj = static_cast<SelfDefineObj *>(target);
    auto str = src.substr(1, src.size() - 2);
    obj->Parser()->Parse(str);
  }
  static void Encode(const void *src, std::string *target)
  {
    SelfDefineObj *obj = const_cast<SelfDefineObj *>(static_cast<const SelfDefineObj *>(src));
    target->append("{").append(obj->Parser()->Encode()).append("}");
  }
};

template <typename T>
struct TypedParser<std::vector<T>>
{
  static void Parse(const std::string &src, void *target)
  {
    std::vector<T> *arr = static_cast<std::vector<T> *>(target);
    arr->clear();
    std::vector<std::string> str_arr = split(src.substr(1, src.size() - 2), '/');
    for(const auto &str : str_arr){
      arr->emplace_back();
      TypedParser<T>::Parse(str, &arr->back());
    }
  }
  static void Encode(const void *src, std::string *target)
  {
    std::vector<T> *arr = const_cast<std::vector<T> *>(static_cast<const std::vector<T> *>(src));
    target->append("[");
    for(const auto &obj : *arr){
      TypedParser<T>::Encode(reinterpret_cast<const void *>(&obj), target);
      target->append("/");
    }
    target->erase(target->size() - 1);
    target->append("]");
  }
};

struct ObjWithParam1
{
  bool flag = true;
  int number = 100;
  SelfDefineObj self_obj{};
  std::vector<std::string> arr{};

  std::unique_ptr<ObjectParser> Parser()
  {
    return ObjectParser::Create(
        "flag", &flag,
        "number", &number,
        "self_obj", &self_obj,
        "arr", &arr);
  }
};

struct ObjWithParam2
{
  bool flag = true;
  std::vector<SelfDefineObj> arr{};

  std::unique_ptr<ObjectParser> Parser()
  {
    return ObjectParser::Create(
        "flag", &flag,
        "obj_arr", &arr);
  }
};

int main()
{
  ObjWithParam1 obj1;
  obj1.Parser()->Parse(R"(flag:true,number:0,self_obj:{name:watt},arr:[one/two])");
  std::cout << obj1.Parser()->Encode() << std::endl;

  ObjWithParam2 obj2;
  obj2.Parser()->Parse(R"(flag:false,obj_arr:[{name:Jam}/{name:Sandra}/{name:Watt}])");
  std::cout << obj2.Parser()->Encode() << std::endl;
}