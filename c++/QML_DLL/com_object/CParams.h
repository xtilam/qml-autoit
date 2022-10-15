#ifndef CPARAMS_H
#define CPARAMS_H

#include <QString>
#include <oaidl.h>

class CArray;
class CObject;
struct BaseObject;
class CVariant;

class FunctionInput
{
    VARIANT* nextVariant();
    bool hasNextParam();
public:
    int index = 0;
    DISPPARAMS* params;
    VARIANT* result;

    FunctionInput(DISPPARAMS*, VARIANT*);

    FunctionInput * setIndex(int index);

    //getter
    FunctionInput * nextToWString(wchar_t **);
    FunctionInput * nextToQString(QString *);
    FunctionInput * nextToNumber(double *);
    FunctionInput * nextToArray(CArray **);
    FunctionInput * nextToObject(CObject **);
	FunctionInput * nextToBool(bool *);
	FunctionInput * nextToCVariant(CVariant *);

    //set result
    FunctionInput * setResultString(wchar_t * str);
    FunctionInput * setResultQString(QString result);
    FunctionInput * setResultNumber(double result);
    FunctionInput * setResultObject(BaseObject *);
	FunctionInput * setResultBool(bool);
	FunctionInput * setResultCVariant(CVariant *);

    int length();
};

#endif // CPARAMS_H
