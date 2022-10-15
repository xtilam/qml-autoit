#ifndef CDEBUGGER_H
#define CDEBUGGER_H

#include "BaseObject.h"
#include <QDebug>

class CDebugger: public BaseObject
{
public:
    inline static bool isEnableDebug = false;
    typedef void(CDebugger::*cdebuggerPrototype)(FunctionInput *);
    void log(FunctionInput * param);
    void logVariant(CVariant * variant, QDebug &debug, const char * trace = "", bool isSpace = true);
    inline static auto prototype = BaseObject::makePrototype<cdebuggerPrototype>
            ({
                 {"log", &CDebugger::log},
                 {"Log", &CDebugger::log},
             });
    inline SPrototype * getPrototype() override{
        return prototype;
    };
};

#endif // CDEBUGGER_H
