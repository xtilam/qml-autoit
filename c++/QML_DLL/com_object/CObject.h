#ifndef COBJECT_H
#define COBJECT_H
#include "BaseObject.h"
#include "CVariant.h"
#include <QMap>

class CObject: public BaseObject
{
public:
    typedef void(CObject::*cobjectPrototype)(FunctionInput *);
    int autoIndex = 0;
    std::map<int, CVariant *> mapData = {};
    std::map<wstring, int> mapIndex = {};

	void onDelete() override;
	void onCreate() override;
    long notPrototypeGetID(wchar_t *key) override;
    void notPrototypeExecute(long key, FunctionInput *params) override;

    void get(FunctionInput * param);
    void set(FunctionInput * param);
    void deleteProperty(FunctionInput * param);

    CVariant * getProperty(wchar_t * key);
    void removeProperty(wchar_t * key);
    void setProperty(wchar_t * key, CVariant * value);


    inline static auto prototype = makePrototype<cobjectPrototype>
            ({
                 {"get", &CObject::get},
                 {"set", &CObject::set},
                 {"delete", &CObject::deleteProperty},
             });


    inline SPrototype *getPrototype() override{
        return prototype;
    };
};

#endif // COBJECT_H
