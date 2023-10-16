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

inline void StringEncode(std::string *target, bool val)
{
  if (val)
  {
    target->append("true");
  }
  else
  {
    target->append("false");
  }
}

inline void StringEncode(std::string *target, int val)
{
  *target += std::to_string(val);
}

/*
  不能按返回值重载，所以需要模板特化指定
*/
template <typename T>
inline T ParseParameter(const std::string &src);

template <>
inline int ParseParameter(const std::string &src)
{
  return std::stoi(src);
}

template <>
inline bool ParseParameter(const std::string &src)
{
  if (src == "true")
  {
    return true;
  }
  else
  {
    return false;
  }
}

struct TypedMemberParser
{
public:
  void (*parse)(const std::string &src, void *target);
  void (*encode)(const void *src, std::string *target);
};

struct MemberParameter
{
  const char *key;
  void *member_ptr;
  TypedMemberParser parser;
};

template <typename T>
class TypedParser
{
public:
  static void Parse(const std::string &src, void *target)
  {
    auto parsed = ParseParameter<T>(src);
    *reinterpret_cast<T *>(target) = parsed;
  }
  static void Encode(const void *src, std::string *target)
  {
    StringEncode(target, *reinterpret_cast<const T *>(src));
  }
};

template class TypedParser<bool>;
template class TypedParser<int>;

template <typename T>
static void AddMembers(MemberParameter *out, const char *key, T *member)
{
  *out = MemberParameter{
      key, member,
      TypedMemberParser{&TypedParser<T>::Parse, &TypedParser<T>::Encode}}; // add member时根据类型添加不同的parse和encode方法
}

template <typename T, typename... Args>
static void AddMembers(MemberParameter *out,
                       const char *key,
                       T *member,
                       Args... args)
{
  AddMembers(out, key, member);
  AddMembers(++out, args...);   // 递归调用，自动匹配完成循环
}

static size_t FindOrEnd(std::string str, size_t start, char delimiter)
{
  size_t pos = str.find(delimiter, start);
  pos = (pos == std::string::npos) ? str.length() : pos;
  return pos;
}

class StructParametersParser
{
public:
  template <typename T, typename... Args>
  static std::unique_ptr<StructParametersParser> Create(const char *first_key,
                                                        T *first_member,
                                                        Args... args)
  {
    std::vector<MemberParameter> members(sizeof...(args) / 2 + 1);
    AddMembers(&members.front(), std::move(first_key), first_member, args...);
    return WrapUnique(new StructParametersParser(std::move(members)));
  }

  void Parse(const std::string &src)
  {
    size_t i = 0;
    while (i < src.length())
    {
      size_t val_end = FindOrEnd(src, i, ',');
      size_t colon_pos = FindOrEnd(src, i, ':');
      size_t key_end = std::min(val_end, colon_pos);
      size_t val_begin = key_end + 1u;
      std::string key(src.substr(i, key_end - i));
      std::string opt_value;
      if (val_end >= val_begin)
        opt_value = src.substr(val_begin, val_end - val_begin);
      i = val_end + 1u;
      bool found = false;
      for (auto &member : members_)
      {
        if (key == member.key)
        {
          found = true;
          member.parser.parse(opt_value, member.member_ptr);
          break;
        }
      }
      // "_" is be used to prefix keys that are part of the string for
      // debugging purposes but not neccessarily used.
      // e.g. WebRTC-Experiment/param: value, _DebuggingString
      if (!found && (key.empty() || key[0] != '_'))
      {
        std::cerr << "No field with key: '" << key
                  << "' (found in trial: \"" << src << "\")";
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
      member.parser.encode(member.member_ptr, &res);
      first = false;
    }
    return res;
  };

private:
  explicit StructParametersParser(
      std::vector<MemberParameter> members) : members_(std::move(members)){};

  std::vector<MemberParameter> members_;
};

struct ObjWithParam
{
  bool flag = true;
  int number = 100;

  std::unique_ptr<StructParametersParser> Parser()
  {
    return StructParametersParser::Create(
        "flag", &flag,
        "number", &number);
  }

  explicit ObjWithParam(const std::string &param)
  {
    Parser()->Parse(param);
  }

  void Print()
  {
    std::cout << "flag = " << flag << " number = " << number << std::endl;
  }
};

int 
main()
{
  ObjWithParam obj("flag:true,number:0");
  obj.Print();
  std::cout << obj.Parser()->Encode() << std::endl;
}