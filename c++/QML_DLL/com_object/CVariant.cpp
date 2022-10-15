#include "CVariant.h"
#include "BaseObject.h"
#include <string>
#include <QDebug>

using namespace std;

const CVariant * CVariant::NULL_VARIANT = new CVariant();

CVariant::~CVariant()
{
    this->clear();
}

void CVariant::setValueFromWString(wchar_t *wstr)
{
    this->clear();
    VARIANT * variant = new VARIANT;
    BaseObject::allocString(variant, wstr);
    this->ptr = (void *)new StringVariant{variant, true};
    this->type = String;
}

void CVariant::setValueFromBSTR(BSTR bstr)
{
    this->clear();
    VARIANT * variant = new VARIANT;
    variant->vt = VT_BSTR;
    variant->bstrVal = bstr;
    this->ptr = (void *)new StringVariant{variant, false};
    this->type = String;
}

void CVariant::setValueFromQString(QString string)
{
    this->setValueFromWString((wchar_t *)string.toStdWString().c_str());
}

void CVariant::setValueFromQStringRef(QString &string)
{
    this->setValueFromWString((wchar_t *)string.toStdWString().c_str());
}

void CVariant::setValueFromNumber(double num)
{
    this->clear();
    this->type = Number;
	this->ptr = new double(num);
}

void CVariant::setValueFromBool(bool val)
{
    this->type = Boolean;
    this->ptr = (void*)val;
}

void CVariant::setValueFromObject(BaseObject *obj)
{
    this->clear();
	if(obj == nullptr) return;
	if(obj->objType == BaseObject::ARRAY){
		this->type = Array;
    }else{
		this->type = Object;
    }
    obj->addRef();
	this->ptr = (void *) obj;
}

void CVariant::setValueFromVariant(VARIANT *variant)
{
	switch (variant->vt) {
    case VT_I4:
    case VT_I2:
    case VT_I1:
    case VT_I8:
        return setValueFromNumber(variant->intVal);
    case VT_R4:
        return setValueFromNumber(variant->fltVal);
    case VT_R8:
        return setValueFromNumber(variant->dblVal);
    case VT_BSTR:
        return setValueFromBSTR(variant->bstrVal);
        return;
    case VT_DISPATCH:
        return setValueFromObject(BaseObject::getBaseObject(variant->pdispVal));
        return;
    }
}

void CVariant::nullValueDelete()
{

}

void CVariant::stringDelete()
{
    StringVariant * strVariant = (StringVariant *)this->ptr;
    if(strVariant->isSystemAlloc){
        BaseObject::freeString(strVariant->variant);
    }
    delete strVariant->variant;
    delete strVariant;
}

void CVariant::numberDelete()
{
    delete (double *)this->ptr;
}

void CVariant::objectDelete()
{
    ((BaseObject *)this->ptr)->releaseRef();
}

void CVariant::nullValueToVariant(VARIANT * v)
{
    v->vt = VT_EMPTY;
}

void CVariant::stringToVariant(VARIANT *v)
{

    v->vt = VT_BSTR;
    v->bstrVal = ((StringVariant *)this->ptr)->variant->bstrVal;
}

void CVariant::numberToVariant(VARIANT *v)
{
    v->vt = VT_R8;
    v->dblVal = *(double *)this->ptr;
}

void CVariant::boolToVariant(VARIANT *v)
{
    v->vt = VT_BOOL;
    v->boolVal = (bool)this->ptr;
}

void CVariant::objectToVariant(VARIANT *v)
{
    auto obj = ((BaseObject *)this->ptr);
    v->vt = VT_DISPATCH;
    v->pdispVal = obj->comobject;
}

void CVariant::getQString(QString * str)
{
	if(this->type != String) return;
    *str = QString::fromStdWString(((StringVariant*)this->ptr)->variant->bstrVal);
}

void CVariant::getNumber(double * num)
{
	if(this->type != Number) return;
	*num = *(double *)this->ptr;
}

void CVariant::getBool(bool * b)
{	if(this->type != Boolean) return;
	*b = (bool) this->ptr;
}

void CVariant::getObject(CObject ** obj)
{
	if(this->type != Object) return;
	BaseObject * baseObj = (BaseObject *)this->ptr;
	if(baseObj->objType != BaseObject::OBJECT) return;
	*obj = (CObject *) this->ptr;
}

void CVariant::getArray(CArray **arr)
{
	if(this->type != Array) return;
	*arr = (CArray *) this->ptr;
}

void CVariant::getVariant(BaseObject ** bo)
{
	if(this->type != Object) return;
	if(this->type != Array) return;
	*bo = (BaseObject *)this->ptr;
}

void CVariant::addRefIfIsObject()
{
    if(this->type != Object && this->type != Array) return;
	((BaseObject *)this->ptr)->addRef();
}

void CVariant::releaseRefIfIsObject()
{
	if(this->type != Object && this->type != Array) return;
	((BaseObject *)this->ptr)->releaseRef();
}

void CVariant::clear()
{
    (this->*listDeleteMethod[type])();
    this->ptr = nullptr;
	this->type = NullValue;
}

void CVariant::applyToVariant(VARIANT * v){
    (this->*listToVariantMethods[type])(v);
}
