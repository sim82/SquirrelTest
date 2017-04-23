#include <iostream>
#include <squirrel/squirrel.h>
//#include "squirrel.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdarg>

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

int compile_file(HSQUIRRELVM v,const char *filename)
{
    std::ifstream ifs(filename);
    if( !ifs.good() )
    {
        return 0;
    }

    sq_compile(v, file_lexfeedASCII,&ifs,filename,1);
    return 1;
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
    std::string inputFile = "/home/sim/src_3dyne/SquirrelTest/test.nut";
    compile_file(v, inputFile.c_str());

    sq_close(v);
    return 0;
}
