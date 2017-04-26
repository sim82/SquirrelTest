#pragma once
#include <squirrel/squirrel.h>
#include <type_traits>
#include <string>


namespace sq
{
class CStackSetter
{
public:
    virtual ~CStackSetter() {}

    virtual void pushInteger(SQInteger v) = 0;
    virtual void pushFloat(SQFloat v) = 0;
    virtual void pushBool(SQBool v) = 0;
    virtual void pushString(std::string const& v) = 0;

    template<typename T>
    typename std::enable_if<std::is_integral<T>::value&&!std::is_same<T,bool>::value,size_t>::type push( T v )
    {
        pushInteger(SQInteger(v));
        return 1;
    }

    template<typename T>
    typename std::enable_if<std::is_floating_point<T>::value,size_t>::type push( T v )
    {
        pushFloat(SQFloat(v));
        return 1;
    }

    template<typename T>
    typename std::enable_if<std::is_same<T,bool>::value,size_t>::type push( T v )
    {
        pushBool(SQBool(v));
        return 1;
    }

    template<typename T>
    typename std::enable_if<std::is_constructible<std::string,T>::value,size_t>::type push( T v )
    {
        pushString(std::string(v));
        return 1;
    }

    template<typename T,typename... Args>
    size_t push(T v, Args...args)
    {
        return push(v) + push(args...);
    }

private:
};
}
