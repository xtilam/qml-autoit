#ifndef CVARIANT_H
#define CVARIANT_H

#include "QString"
#include <oaidl.h>
#include "CParams.h"

struct BaseObject;

class CVariant
{
public:
    typedef void(CVariant::*clearMethod)();
    typedef void(CVariant::*toVariantMethod)(VARIANT *);

    struct StringVariant{
        VARIANT * variant;
        bool isSystemAlloc;
    };

    enum VariantType{
        NullValue = 0,
        String,
        Number,
        Boolean,
        Object,
        Array,
    };

	CVariant(){};
    ~CVariant();

    VariantType type = VariantType::NullValue;
    void * ptr = nullptr;
	static const CVariant * NULL_VARIANT;
    // set value
    void setValueFromWString(wchar_t * wstr);
    void setValueFromBSTR(BSTR bstr);
	void setValueFromQString(QString string);
    void setValueFromQStringRef(QString &string);
    void setValueFromNumber(double num);
    void setValueFromBool(bool val);
    void setValueFromObject(BaseObject * obj);
    void setValueFromVariant(VARIANT * variant);

    // delete CVariant
    void nullValueDelete();
    void stringDelete();
    void numberDelete();
    void objectDelete();

    // convert to VARIANT
    void nullValueToVariant(VARIANT *);
    void stringToVariant(VARIANT *);
    void numberToVariant(VARIANT *);
    void boolToVariant(VARIANT *);
    void objectToVariant(VARIANT *);

	void getQString(QString *);
	void getNumber(double *);
	void getBool(bool *);
	void getObject(CObject **);
	void getArray(CArray **);
	void getVariant(BaseObject **);

	void addRefIfIsObject();
	void releaseRefIfIsObject();


    inline static clearMethod listDeleteMethod[] = {
        &CVariant::nullValueDelete,
        &CVariant::stringDelete,
        &CVariant::numberDelete,
        &CVariant::nullValueDelete,
        &CVariant::objectDelete,
        &CVariant::objectDelete,
    };

    inline static toVariantMethod listToVariantMethods [] = {
        &CVariant::nullValueToVariant,
        &CVariant::stringToVariant,
        &CVariant::numberToVariant,
        &CVariant::boolToVariant,
        &CVariant::objectToVariant,
        &CVariant::objectToVariant,
    };

    void clear();
    void applyToVariant(VARIANT *);
};

#endif // CVARIANT_H
