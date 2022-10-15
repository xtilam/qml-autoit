#ifndef BASEOBJECT_H
#define BASEOBJECT_H
#include <oaidl.h>
#include <QString>
#include "CVariant.h"
#include <string>
#include <map>
#include <QDebug>

using namespace std;

struct BaseObject
{
    static void comObjectOnDelete(BaseObject *obj);
    static long comObjectGetID(BaseObject *obj, wchar_t * key);
    static void comObjectOnExecute(BaseObject * obj, long id, DISPPARAMS* pDispParams, VARIANT* pVarResult);

    inline static void * functions[]= {
        (void *)comObjectOnDelete,
        (void *)comObjectGetID,
        (void *)comObjectOnExecute,
    };

public:
    inline BaseObject(){};

    typedef void(*TCreateMethod)(void* data);
    typedef void(*TDeleteMethod)(void* data);
    typedef long(*TGetIDMethod)(void* data, wchar_t*);
    typedef void(*TExecuteMethod)(void* data, DISPPARAMS* pDispParams, VARIANT* pVarResult);

    typedef LPDISPATCH (*TInitComObjectType)(void ** methods, void * data);
    typedef BaseObject * (*TGetCObject)(LPDISPATCH obj);
    typedef void (*TAllocString)(VARIANT * v, wchar_t * str);
    typedef void (*TDeleteObject)(LPDISPATCH);
    typedef void (*TAddRef)(LPDISPATCH);
    typedef void (*TRelaseRef)(LPDISPATCH);
    typedef void (*TFreeString)(VARIANT *);

    typedef void(BaseObject::*prototypeType)(FunctionInput *);

    enum ObjectType{
        NULL_VALUE = 0,
        OBJECT,
        ARRAY,
        BASE_OBJECT,
		UI_OBJECT,
    };

    struct SPrototype{
        int length;
        prototypeType* list;
        map<wstring, u_short> map;
    };
    template<class T>
    struct PrototypeInput
    {
        string name;
        T function;
    };

    LPDISPATCH comobject;
    ObjectType objType = ObjectType::BASE_OBJECT;

    template<typename T>
    inline static T *init(){
		auto obj = (BaseObject *)new T;
        obj->comobject = initComObjectMethod(functions, obj);
        obj->onCreate();
//        qDebug() << "init object" << obj;
        return (T*)obj;
    };

    template<typename T>
    inline static SPrototype *makePrototype(std::initializer_list<PrototypeInput<T>> items){
        u_short length = items.size();
        T * prototype = new T[length];

        auto result = new SPrototype{
            length,
            (prototypeType*)prototype,
            {}
        };

        u_short index = 0;
        for(PrototypeInput<T> const &item: items){
            prototype[index] = item.function;
            result->map[QString::fromStdString(item.name.c_str()).toStdWString()] = ++index;
        }
        return result;
    };

    inline static TInitComObjectType initComObjectMethod = nullptr;
    inline static TGetCObject getBaseObject = nullptr;
    inline static TAllocString allocString = nullptr;
    inline static TDeleteMethod deleteObject = nullptr;
    inline static TAddRef addRefObject = nullptr;
    inline static TRelaseRef releaseRefObject = nullptr;
    inline static TFreeString freeString = nullptr;

    inline virtual SPrototype * getPrototype(){ return nullptr; };
    inline virtual void onDelete(){};
    inline virtual void onCreate(){};
    inline virtual long notPrototypeGetID(wchar_t *key){ return 0;};
    inline virtual void notPrototypeExecute(long key, FunctionInput *params){};

	inline void addRef(){
		addRefObject(this->comobject);
	}
	inline void releaseRef(){
		releaseRefObject(this->comobject);
	}
};
#endif // BASEOBJECT_H
