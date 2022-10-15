#include "BaseObject.h"
#include <QString>
#include "CObject.h"

void BaseObject::comObjectOnDelete(BaseObject *obj)
{
//    qDebug() << "object delete" << obj;
    obj->onDelete();
}

long BaseObject::comObjectGetID(BaseObject *obj, wchar_t *key)
{
    auto prorotype = obj->getPrototype();
    long id = 0;

    if(prorotype){
        id = obj->getPrototype()->map[key];
        if(id != 0) return id;
    }

    return obj->notPrototypeGetID(key) * -1;
}

void BaseObject::comObjectOnExecute(BaseObject *obj, long id, DISPPARAMS *pDispParams, VARIANT *pVarResult)
{
    FunctionInput params(pDispParams, pVarResult);

    if(id==0) return;

    auto prototype = obj->getPrototype();

    if(id > 0){
        if(id <= prototype->length){
            return (obj->*(prototype->list[id - 1]))(&params);
        }

    }else{
        obj->notPrototypeExecute(id * -1, &params);
	}
}
