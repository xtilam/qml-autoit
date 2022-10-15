#include "CDebugger.h"
#include <QDebug>
#include "CArray.h"
#include "CObject.h"

void CDebugger::log(FunctionInput *param)
{
    if(!isEnableDebug) return;
    int length = param->length();
    auto debug = qDebug().noquote().nospace();
    CVariant variant;
    for(int i=0; i<length; i++){
        param->nextToCVariant(&variant);
        logVariant(&variant, debug);
    }
}

void CDebugger::logVariant(CVariant *variant, QDebug &debug, const char * trace, bool isSpace)
{
    switch (variant->type) {
    case CVariant::String:{
        QString str;
        variant->getQString(&str);
        if(str.isEmpty()) break;
        debug << str;
        break;
    }
    case CVariant::Number:{
        double num;
        variant->getNumber(&num);
        debug << num;
        break;
    }

    case CVariant::NullValue:
        debug << "NULL";
        break;
    case CVariant::Array:
    {
        auto arr = (CArray*)variant->ptr;
        debug << "Array(" << arr->items.size() << ")[\n";
        int index = 0;
        auto nextTrace =  QString::fromStdString(trace).append("  ").toStdString();
        for(auto item: arr->items){
            debug << nextTrace.c_str() << index++ << ": ";
            logVariant(item, debug, nextTrace.c_str(), false);
            debug << ",\n";
        }
        debug << trace << "]";
        break;
    }

    case CVariant::Object:
    {
        BaseObject * bobj = (BaseObject *)variant->ptr;
        if(bobj->objType == BaseObject::OBJECT){
            debug << "Object {\n";
            CObject * obj = (CObject *) bobj;
            auto nextTrace =  QString::fromStdString(trace).append("  ").toStdString();
            for(auto item: obj->mapIndex){
                debug << nextTrace.c_str() << item.first << ": ";
                logVariant(obj->mapData[item.second], debug, nextTrace.c_str(), false);
                debug << ",\n";
            }
            debug << trace << "}";
        }else{
            debug << "Object(" << bobj << ")";

        }
        break;
    }

    case CVariant::Boolean:
        debug << (((bool)variant->ptr) ? "true " : "false");
        break;
    }

    if(isSpace){
        debug << " ";
    }
}
