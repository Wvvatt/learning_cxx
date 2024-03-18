#include <iostream>
#include <type_traits>
#include <memory>
#include <vector>
#include <sstream>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

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
    void (*parse)(const rapidjson::Value &src, void *target);
    void (*encode)(rapidjson::Value &target, void *src, rapidjson::Document::AllocatorType &allocator);
};

template <typename T, typename = void>
struct TypedParser
{
    static void Parse(const rapidjson::Value &val, void *target);
    static void Encode(rapidjson::Value &target, void *src, rapidjson::Document::AllocatorType &allocator);
};

template <typename T>
struct is_vector : std::false_type {};

template <typename T, typename Alloc>
struct is_vector<std::vector<T, Alloc>> : std::true_type {};

template <typename T>
struct is_class_not_vector {
    static constexpr bool value = std::is_class<T>::value && !is_vector<T>::value;
};

template <typename T>
static void AddMembers(MemberParameter *out, const char *key, T *member)
{
    static_assert(
        std::is_arithmetic<T>::value ||               // bool,char,int,uint,int64,uint64,float,double
        std::is_same<T, std::string>::value ||          
        std::is_class<T>::value ||
        is_vector<T>::value
    );
    *out = MemberParameter{
        key, member,
        &TypedParser<T>::Parse,
        &TypedParser<T>::Encode};
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

    bool Parse(const rapidjson::Value &val)
    {
        if (!val.IsObject())
        {
            return false;
        }
        for (auto &&m : members_)
        {
            if (val.HasMember(m.key) && !val[m.key].IsNull())
            {
                m.parse(val[m.key], m.member_ptr);
            }
        }
        return true;
    };

    void Encode(rapidjson::Value &val, rapidjson::Document::AllocatorType &allocator) const
    {
        val.SetObject();
        for (auto &&m : members_)
        {
            rapidjson::Value name(rapidjson::kStringType);
            name.SetString(m.key, allocator);

            rapidjson::Value obj;
            m.encode(obj, m.member_ptr, allocator);

            val.AddMember(name, obj, allocator);
        }
    };

private:
    explicit ObjectParser(
        std::vector<MemberParameter> members) : members_(std::move(members)){};
    std::vector<MemberParameter> members_;
};

template <>
struct TypedParser<bool>
{
    static void Parse(const rapidjson::Value &val, void *target)
    {
        if (val.IsTrue())
        {
            *static_cast<bool *>(target) = true;
        }
        else if (val.IsFalse())
        {
            *static_cast<bool *>(target) = false;
        }
        else
        {
            std::cerr << "bool parse failed!" << std::endl;
        }
    }
    static void Encode(rapidjson::Value &target, void *src, rapidjson::Document::AllocatorType &allocator)
    {
        ;
        target.SetBool(*static_cast<bool *>(src));
    }
};

template <>
struct TypedParser<std::string>
{
    static void Parse(const rapidjson::Value &val, void *target)
    {
        if (val.IsString())
        {
            *static_cast<std::string *>(target) = val.GetString();
        }
    }
    static void Encode(rapidjson::Value &target, void *src, rapidjson::Document::AllocatorType &allocator)
    {
        target.SetString(static_cast<std::string *>(src)->c_str(), allocator);
    }
};

template <typename T>
struct TypedParser<T, typename std::enable_if<is_class_not_vector<T>::value>::type>
{
    static void Parse(const rapidjson::Value &val, void *target)
    {
        static_cast<T *>(target)->Parser()->Parse(val);
    }
    static void Encode(rapidjson::Value &target, void *src, rapidjson::Document::AllocatorType &allocator)
    {
        static_cast<T *>(src)->Parser()->Encode(target, allocator);
    }
};

template <typename T>
struct TypedParser<T, typename std::enable_if<is_vector<T>::value>::type>
{
    static void Parse(const rapidjson::Value &val, void *target)
    {
        if (!val.IsArray()){
            return;
        }
        T *arr = static_cast<T *>(target);
        arr->clear();
        for (rapidjson::SizeType i = 0; i < val.Size(); ++i){
            arr->emplace_back();
            TypedParser<typename T::value_type>::Parse(val[i], &arr->back());
        }
    }
    static void Encode(rapidjson::Value &target, void *src, rapidjson::Document::AllocatorType &allocator)
    {
        target.SetArray();
        T *arr = static_cast<T *>(src);
        for(auto iter = arr->begin(); iter != arr->end(); ++iter){
            rapidjson::Value val;
            TypedParser<typename T::value_type>::Encode(val, iter.operator->(), allocator);
            target.PushBack(val, allocator);
        }
    }
};

template <>
struct TypedParser<int>
{
    static void Parse(const rapidjson::Value &val, void *target)
    {
        if (val.IsInt()){
            *static_cast<int *>(target) = val.GetInt();
        }
        else{
            std::cerr << "parse int failed!" << std::endl;
        }
    }
    static void Encode(rapidjson::Value &target, void *src, rapidjson::Document::AllocatorType &allocator)
    {
        target.SetInt(*static_cast<int *>(src));
    }
};

template <>
struct TypedParser<uint>
{
    static void Parse(const rapidjson::Value &val, void *target)
    {
        if (val.IsUint()){
            *static_cast<uint *>(target) = val.GetUint();
        }
        else{
            std::cerr << "parse int failed!" << std::endl;
        }
    }
    static void Encode(rapidjson::Value &target, void *src, rapidjson::Document::AllocatorType &allocator)
    {
        target.SetUint(*static_cast<uint *>(src));
    }
};

template <>
struct TypedParser<int64_t>
{
    static void Parse(const rapidjson::Value &val, void *target)
    {
        if (val.IsInt64()){
            *static_cast<int64_t *>(target) = val.GetInt64();
        }
        else{
            std::cerr << "parse int failed!" << std::endl;
        }
    }
    static void Encode(rapidjson::Value &target, void *src, rapidjson::Document::AllocatorType &allocator)
    {
        target.SetInt64(*static_cast<int64_t *>(src));
    }
};

template <>
struct TypedParser<uint64_t>
{
    static void Parse(const rapidjson::Value &val, void *target)
    {
        if (val.IsUint64()){
            *static_cast<uint64_t *>(target) = val.GetUint64();
        }
        else{
            std::cerr << "parse int failed!" << std::endl;
        }
    }
    static void Encode(rapidjson::Value &target, void *src, rapidjson::Document::AllocatorType &allocator)
    {
        target.SetUint64(*static_cast<uint64_t *>(src));
    }
};

struct Obj1
{
    std::string name;
    int age;

    std::unique_ptr<ObjectParser> Parser()
    {
        return ObjectParser::Create(
            "name", &name,
            "age", &age);
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
            "obj1", &obj1,
            "arr", &arr);
    }
};

int main()
{
    rapidjson::Document doc;
    doc.Parse(R"RSL(
    {
      "flag": true,
      "number": 100,
      "obj1": {
        "name": "watt",
        "age": 17
      },
      "arr": [
        "one",
        "two",
        "three"
      ]
    }
    )RSL");
    Obj2 obj;
    obj.Parser()->Parse(doc);

    rapidjson::Document doc_print;
    obj.Parser()->Encode(doc_print, doc_print.GetAllocator());

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer, rapidjson::UTF8<>, rapidjson::UTF8<>, rapidjson::CrtAllocator, rapidjson::kWriteDefaultFlags> writer(buffer);
    doc_print.Accept(writer);
    std::cout << buffer.GetString();

    return 0;
}