#ifndef ENUMSTRING_HEADER
#define ENUMSTRING_HEADER

#include <string>
#include <map>
#include <cassert>

#define Begin_Enum_String(E)              \
    template <>                           \ 
    void EnumString<E>::RegisterAll()        \
    
#define Enum_String(Enum, String)                     \
    RegisterEnumerator(Enum, String);


template <typename E>
class EnumString
{
public:
    typedef std::map<std::string, E> AssocMap;
    static void RegisterEnumerator(const E e, const std::string &eStr)
    {
        const bool bRegistered = GetMap().insert(typename AssocMap::value_type(eStr, e)).second;
        assert(bRegistered);
        (void)sizeof(bRegistered);
    }

    static const std::string &From(const E e)
    {
        for (;;) // Code block
        {
            typename AssocMap::const_iterator i;
            for (i = GetMap().begin(); i != GetMap().end(); ++i)
                if ((*i).second == e)
                    break;

            if (i == GetMap().end())
                break;

            typename AssocMap::const_iterator j(i);
            for (++j; j != GetMap().end(); ++j)
                if ((*j).second == e)
                    break;

            if (j != GetMap().end())
                break;

            return (*i).first;
        }

        static const std::string dummy;
        return dummy;
    }
    static const bool To(E &e, const std::string &str)
    {
        const typename AssocMap::const_iterator itr(GetMap().find(str));
        if (itr != GetMap().end())
        {
            e = (*itr).second;
            return true;
        }

        return false;
    }

    static void RegisterAll();

private:
    static AssocMap &GetMap()
    {
        static AssocMap assocMap;
        static bool bFirstAccess = true;
        if (bFirstAccess)
        {
            bFirstAccess = false;
            RegisterAll();
            assert(!assocMap.empty());
        }

        return assocMap;
    }
};

enum ConnMode_e
{
    kListener = 0,
    kCaller
};


Begin_Enum_String(ConnMode_e)
{
    Enum_String(kListener,  "listener");
    Enum_String(kCaller,    "caller");
}


#endif