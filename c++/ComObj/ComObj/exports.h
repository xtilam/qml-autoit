#ifndef EXPORTS_H
#define EXPORTS_H

#include <oaidl.h>
#include "comobj.h"

LPDISPATCH initComObjectMethod(void** methods, void* data);
void* getBaseCObject(ComObject * obj);
void allocString(VARIANT* v, wchar_t*);
void deleteObject(ComObject*);
void addRef(ComObject *obj);
void releaseRef(ComObject *obj);
void freeString(VARIANT* v);

inline void* dllMethods[] = {
    (void*)initComObjectMethod,
    (void*)getBaseCObject,
    (void*)allocString,
    (void*)deleteObject,
    (void*)addRef,
    (void*)releaseRef,
    (void*)freeString,
};

extern "C" __declspec(dllexport) void** __stdcall getAllMethods();

#endif // EXPORTS_H
