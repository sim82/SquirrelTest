#include <iostream>
#include <squirrel/squirrel.h>
#include <iostream>
#include <fstream>

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


int main(int argc, char *argv[])
{
    HSQUIRRELVM v;
    v = sq_open(1024); //creates a VM with initial stack size 1024
    sq_setcompilererrorhandler(v, ErrorHandler);
    //do some stuff with squirrel here
    compile_file(v, "/home/sim/src/SquirrelTest/test.nut");
    sq_close(v);
    return 0;
}
