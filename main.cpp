#include "CVm.h"

//#include "squirrel.h"
#include <iostream>

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


int main(int argc, char *argv[])
{
    sq::CVm vm;
    //do some stuff with squirrel here

//	std::string inputFile = "C:\\src_3dyne\\SquirrelTest\\test.nut";
    std::string inputFile = "/home/sim/src/SquirrelTest/test.nut";
    auto bRes = vm.compileFile(inputFile);

    auto type = sq_gettype(vm, -1);
    std::cout << "type: " << std::hex << type << "\n";

    if( !bRes )
    {
        throw std::runtime_error( "compile failed" );
    }

    vm.pushRoottable();
    auto num = vm.push(666, 123.4, true, "bla");
//    vm.push(666);
//    vm.push(123.4);
//    vm.push(true);
//    vm.push("bla");

#if 0
    {
        int v0;
        float v1;
        bool v2;
        std::string v3;
        vm.popMulti(v0,v1,v2,v3);

        std::cout << v0 << v1 << v2 << v3 << std::endl;
    }
#endif
    vm.call(num+1);
    type = sq_gettype(vm,-1);
    std::cout << "type: " << TypeToString(type) << "\n";
    std::cout << "ret: " << std::dec << vm.isA<std::string>(-1) << " " << std::dec << vm.isA<bool>(-1) << " " << vm.get<std::string>(-1) << std::endl;
    std::cout << "ret: " << std::dec << vm.isA<bool>(-1) << " " << vm.get<bool>(-1) << std::endl;
    vm.pop(1);
    return 0;
}
