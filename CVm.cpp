#include "CVm.h"

#include <iostream>
#include <fstream>

#include <cstdio>
#include <cstdarg>
#include <stdexcept>


using namespace sq;

CVm::CVmGuard::CVmGuard(size_t initStacksize)
    : vm_(sq_open(SQInteger(initStacksize)))
{}

CVm::CVmGuard::~CVmGuard()
{
    if(vm_)
    {
        sq_close(vm_);
    }
}

HSQUIRRELVM CVm::CVmGuard::get() const
{
    return vm_;
}

sq::CVm::CVmGuard::operator HSQUIRRELVM()
{
    return vm_;
}

CVm::CVm(size_t initStacksize)
    : vm_(initStacksize)
{
    sq_setcompilererrorhandler(vm_, errorHandler);
    sq_setprintfunc(vm_, printFunc, printFunc);
}

bool CVm::compileFile(const std::__cxx11::string &filename)
{
    std::ifstream ifs(filename);
    if( !ifs.good() )
    {
        return false;
    }

    return SQ_SUCCEEDED(sq_compile(vm_, fileLexfeedAscii,&ifs,filename.c_str(),1));
}

void CVm::pushRoottable()
{
    sq_pushroottable(vm_);
}

void CVm::pop(size_t num)
{
    sq_pop(vm_, SQInteger(num));
}

void CVm::call(size_t numArgs)
{
    auto res = sq_call(vm_, SQInteger(numArgs), SQTrue, SQTrue);

    if( !SQ_SUCCEEDED(res))
    {
        std::cout << "sq_call failed: " << res << std::endl;
    }
}

std::pair<SQInteger, bool> CVm::getInteger(SQInteger idx)
{
    SQInteger i;
    auto res = sq_getinteger(vm_,idx,&i);

    return {i, SQ_SUCCEEDED(res)};
}

std::pair<SQFloat, bool> CVm::getFloat(SQInteger idx)
{
    SQFloat i;
    auto res = sq_getfloat(vm_,idx,&i);

    return {i, SQ_SUCCEEDED(res)};
}

std::pair<SQBool, bool> CVm::getBool(SQInteger idx)
{
    SQBool i;
    auto res = sq_getbool(vm_,idx,&i);

    return {i, SQ_SUCCEEDED(res)};
}

std::pair<std::__cxx11::string, bool> CVm::getString(SQInteger idx)
{
    const SQChar *s;
    SQInteger n;
    auto res = sq_getstringandsize(vm_, idx, &s, &n);

    if(SQ_FAILED(res))
    {
        return {std::string(),false};
    }
    return {std::string(s, n), true};
}

SQObjectType CVm::getType(SQInteger idx)
{
    return sq_gettype(vm_, idx);
}

void CVm::pushInteger(SQInteger i)
{
    sq_pushinteger(vm_, i);
}

void CVm::pushFloat(SQFloat i)
{
    sq_pushfloat(vm_, i);
}

void CVm::pushBool(SQBool i)
{
    sq_pushbool(vm_, i);
}

void CVm::pushString(const std::__cxx11::string &str)
{
    sq_pushstring(vm_, str.data(), SQInteger(str.size()));
}

void CVm::errorHandler(HSQUIRRELVM v, const SQChar *desc, const SQChar *source, SQInteger line, SQInteger column)
{
    std::cerr << "squirrel error\n";
    std::cerr << desc << "\n";
    std::cerr << source << ":" << line << ":" << column << "\n";

}

void CVm::printFunc(HSQUIRRELVM v, const SQChar *s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stdout, s, args);
    va_end(args);
    fputc('\n', stdout);
}

SQInteger CVm::fileLexfeedAscii(SQUserPointer file)
{

    auto &is = *reinterpret_cast<std::istream*>(file);

    auto c = is.get();

    if( is.eof() )
    {
        return 0;
    }
    //    std::cout << (char)c;
    return c;
}

sq::CVm::operator HSQUIRRELVM()
{
    return vm_;
}
