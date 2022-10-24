#ifndef UIOBJECT_H
#define UIOBJECT_H

#include "BaseObject.h"
#include <map>

class UIObject : public BaseObject
{
public:
    int autoIndex = 1;
    int methodId = 0;

    map<int, UIObject *> mapObject;
    map<wstring, int> mapID;

    inline SPrototype * getPrototype() override{
        return nullptr;
    };

    UIObject *add(wchar_t * key);

public:
    long notPrototypeGetID(wchar_t *key) override;
    void notPrototypeExecute(long key, FunctionInput *params) override;
    void onDelete() override;
    void clear();
    void setMethodID(int id);
    void onCreate() override{
        this->objType = UI_OBJECT;
    };
};

#endif // UIOBJECT_H
