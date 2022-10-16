#ifndef CARRAY_H
#define CARRAY_H
#include "BaseObject.h"
#include <vector>

class CArray : public BaseObject
{


public:
    vector<CVariant *> items;
    typedef void (CArray::*CArrayPrototype)(FunctionInput *);

    void onCreate() final override;
    void onDelete() final override;

    void _length(FunctionInput *params);
    uint length();

    void _get(FunctionInput *params);
    CVariant * get(uint index);

    void _push(FunctionInput *params);
    void push(vector<CVariant *> variants);

    void _unshift(FunctionInput *params);
    void unshift(vector<CVariant *> variants);

    void _splice(FunctionInput *params);
    void splice(uint start, uint nItem, vector<CVariant *> variants);

    void _shift(FunctionInput *params);
    void shift();

    void _pop(FunctionInput *params);
    void pop();

    void _isArray(FunctionInput *params){
        params->setResultBool(true);
    };
    static inline SPrototype *prototype = makePrototype<CArrayPrototype>({
        {"length", &CArray::_length},
        {"get", &CArray::_get},
        {"push", &CArray::_push},
        {"unshift", &CArray::_unshift},
        {"splice", &CArray::_splice},
        {"shift", &CArray::_shift},
        {"pop", &CArray::_pop},
        {"isArray", &CArray::_isArray}
    });

    SPrototype *getPrototype() override;
};

#endif // CARRAY_H
