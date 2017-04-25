#include <iostream>
#include <squirrel/squirrel.h>
//#include "squirrel.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <cstdio>
#include <cstdarg>

std::string TypeToString( SQObjectType const& type )
{

    switch(type)
    {
    case OT_NULL          : return "OT_NULL";
    case OT_INTEGER       : return "OT_INTEGER";
    case OT_FLOAT         : return "OT_FLOAT";
    case OT_BOOL          : return "OT_BOOL";
    case OT_STRING        : return "OT_STRING";
    case OT_TABLE         : return "OT_TABLE";
    case OT_ARRAY         : return "OT_ARRAY";
    case OT_USERDATA      : return "OT_USERDATA";
    case OT_CLOSURE       : return "OT_CLOSURE";
    case OT_NATIVECLOSURE : return "OT_NATIVECLOSURE";
    case OT_GENERATOR     : return "OT_GENERATOR";
    case OT_USERPOINTER   : return "OT_USERPOINTER";
    case OT_THREAD        : return "OT_THREAD";
    case OT_FUNCPROTO     : return "OT_FUNCPROTO";
    case OT_CLASS         : return "OT_CLASS";
    case OT_INSTANCE      : return "OT_INSTANCE";
    case OT_WEAKREF       : return "OT_WEAKREF";
    case OT_OUTER         : return "OT_OUTER";
    default: return "<unknown>";

    }
}

void ErrorHandler(HSQUIRRELVM v,const SQChar * desc,const SQChar * source,
                        SQInteger line,SQInteger column)
{
    std::cerr << "squirrel error\n";
    std::cerr << desc << "\n";
    std::cerr << source << ":" << line << ":" << column << "\n";

}

SQInteger file_lexfeedASCII(SQUserPointer file)
{

    auto &is = *((std::istream*)file);

    auto c = is.get();

    if( is.eof() )
    {
        return 0;
    }
//    std::cout << (char)c;
    return c;
}

bool compile_file(HSQUIRRELVM v,const char *filename)
{
    std::ifstream ifs(filename);
    if( !ifs.good() )
    {
        return false;
    }

    return SQ_SUCCEEDED(sq_compile(v, file_lexfeedASCII,&ifs,filename,1));
}
void printFunc(HSQUIRRELVM v, const SQChar *s, ...)
{
    va_list args;
    va_start(args, s);
    vfprintf(stdout, s, args);
    va_end(args);
}

int main(int argc, char *argv[])
{
    HSQUIRRELVM v;
    v = sq_open(1024); //creates a VM with initial stack size 1024
    sq_setcompilererrorhandler(v, ErrorHandler);
    sq_setprintfunc(v, printFunc, printFunc);

    //do some stuff with squirrel here

//	std::string inputFile = "C:\\src_3dyne\\SquirrelTest\\test.nut";
    std::string inputFile = "/home/sim/src/SquirrelTest/test.nut";
    auto bRes = compile_file(v, inputFile.c_str());

    auto type = sq_gettype(v, -1);
    std::cout << "type: " << std::hex << type << "\n";

    if( !bRes )
    {
        throw std::runtime_error( "compile failed" );
    }

    sq_pushroottable(v);
//    sq_pushinteger(v, 1);
    auto res = sq_call(v, 1, SQTrue, SQTrue);
    std::cout << "success: " << SQ_SUCCEEDED(res) << "\n";
    type = sq_gettype(v,-1);
    std::cout << "type: " << TypeToString(type) << "\n";
    SQInteger ret;
    sq_getinteger(v, -1, &ret);
    std::cout << "ret: " << std::dec << ret << std::endl;
    sq_pop(v, 1);
    sq_close(v);
    return 0;
}
