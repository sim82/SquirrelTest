#pragma once
#include "CStackSetter.h"
#include "CStackGetter.h"
#include <squirrel/squirrel.h>

namespace sq
{

class CVm : public CStackGetter, public CStackSetter
{
    // going for a strict 'rule of zero' design here
    class CVmGuard
    {
    public:
        CVmGuard() = delete;
        CVmGuard(CVmGuard const&) = delete;
        CVmGuard(CVmGuard &&) = delete;
        CVmGuard& operator=(CVmGuard const&) = delete;
        CVmGuard& operator=(CVmGuard &&) = delete;

        CVmGuard( size_t initStacksize = 1024 );

        ~CVmGuard();
        HSQUIRRELVM get() const;
        operator HSQUIRRELVM();
    private:
        HSQUIRRELVM vm_;
    };

public:
    CVm( size_t initStacksize = 1024 );

    bool compileFile(std::string const& filename);

    operator HSQUIRRELVM();

    void pushRoottable();
    void pop(size_t num) override;

    void call(size_t numArgs = 1);

    std::pair<SQInteger,bool> getInteger(SQInteger idx) override;

    std::pair<SQFloat,bool> getFloat(SQInteger idx) override;

    std::pair<SQBool,bool> getBool(SQInteger idx) override;

    std::pair<std::string,bool> getString(SQInteger idx) override;

    SQObjectType getType(SQInteger idx) override;

    void pushInteger(SQInteger i) override;

    void pushFloat(SQFloat i) override;

    void pushBool(SQBool i) override;

    void pushString(std::string const& str) override;

private:
    static void errorHandler(HSQUIRRELVM v,const SQChar * desc,const SQChar * source,
                            SQInteger line,SQInteger column);

    static void printFunc(HSQUIRRELVM v, const SQChar *s, ...);

    static SQInteger fileLexfeedAscii(SQUserPointer file);

    CVmGuard vm_;

};
}
