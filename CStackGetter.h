#pragma once
#include <squirrel/squirrel.h>
#include <type_traits>
#include <string>
#include <tuple>

namespace sq
{
class CStackGetter
{
public:
    virtual ~CStackGetter() {}

    virtual std::pair<SQInteger,bool> getInteger(SQInteger idx) = 0;
    virtual std::pair<SQFloat,bool> getFloat(SQInteger idx) = 0;
    virtual std::pair<SQBool,bool> getBool(SQInteger idx) = 0;
    virtual std::pair<std::string,bool> getString(SQInteger idx) = 0;
    virtual SQObjectType getType(SQInteger idx) = 0;
    virtual void pop(size_t num) = 0;


    template<typename T>
    typename std::enable_if<std::is_integral<T>::value&&!std::is_same<T,bool>::value,bool>::type isA(SQInteger idx)
    {
        return this->getType(idx) == OT_INTEGER;
    }

    template<typename T>
    typename std::enable_if<std::is_floating_point<T>::value,bool>::type isA(SQInteger idx)
    {
        return this->getType(idx) == OT_FLOAT;
    }

    template<typename T>
    typename std::enable_if<std::is_same<T,bool>::value,bool>::type isA(SQInteger idx)
    {
        return this->getType(idx) == OT_BOOL;
    }

    template<typename T>
    typename std::enable_if<std::is_same<T,std::string>::value,bool>::type isA(SQInteger idx)
    {
        return this->getType(idx) == OT_STRING;
    }

    template<typename T>
    typename std::enable_if<std::is_integral<T>::value&&!std::is_same<T,bool>::value,T>::type get(SQInteger idx)
    {
        SQInteger i;
        bool bSuccess;

        std::tie(i,bSuccess) = this->getInteger(idx);
        if( !bSuccess )
        {
            return T();
        }
        return T(i);
    }

    template<typename T>
    typename std::enable_if<std::is_floating_point<T>::value,T>::type get(SQInteger idx)
    {
        SQFloat i;
        bool bSuccess;

        std::tie(i,bSuccess) = this->getFloat(idx);
        if( !bSuccess )
        {
            return T();
        }
        return T(i);
    }

    template<typename T>
    typename std::enable_if<std::is_same<T, bool>::value,T>::type get(SQInteger idx)
    {
        SQBool i;
        bool bSuccess;

        std::tie(i,bSuccess) = this->getInteger(idx);
        if( !bSuccess )
        {
            return T();
        }
        return T(i);
    }

    template<typename T>
    typename std::enable_if<std::is_same<T, std::string>::value,T>::type get(SQInteger idx)
    {
        std::string s;
        bool bSuccess;

        std::tie(s,bSuccess) = this->getString(idx);
        if( !bSuccess )
        {
            return T();
        }
        return s;
    }

    template<typename T,typename... Args>
    void popMulti(T & v, Args&...args)
    {
        popMulti(args...);

        v = get<T>(-1);
        this->pop(1);
        //return push(v) + push(args...);
    }
    template<typename T>
    void popMulti(T & v)
    {
        v = get<T>(-1);
        this->pop(1);
    }
};

}
