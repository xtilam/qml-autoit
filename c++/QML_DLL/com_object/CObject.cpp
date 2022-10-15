#include "CObject.h"

void CObject::onDelete()
{
    for(auto &item: this->mapData){
        delete item.second;
    }
    delete this;
}

void CObject::onCreate()
{
    this->objType = OBJECT;
}

long CObject::notPrototypeGetID(wchar_t *key)
{
    return this->mapIndex[key];
}

void CObject::notPrototypeExecute(long key, FunctionInput *params)
{
    auto variant = this->mapData[key];
    if(!variant) return;
    params->setResultCVariant(variant);
}

void CObject::get(FunctionInput *params)
{

    wchar_t * key;
    params->nextToWString(&key);
    auto variant = this->getProperty(key);
    if(!variant) return;
    params->setResultCVariant(variant);
}

void CObject::set(FunctionInput *params)
{
    wchar_t * key;
    CVariant * v = new CVariant;

    params->nextToWString(&key)
            ->nextToCVariant(v);

    this->setProperty(key, v);
    params->setResultObject(this);
}

void CObject::deleteProperty(FunctionInput *params)
{
    wchar_t * key;
    params->nextToWString(&key);
    this->removeProperty(key);
}

CVariant *CObject::getProperty(wchar_t *key)
{
    auto index = this->mapIndex[key];
    return index == 0 ? this->mapData[index] : nullptr;
}

void CObject::removeProperty(wchar_t *key)
{
    auto index = this->mapIndex[key];

    if(index == 0) return;

    delete this->mapData[index];
    this->mapData.erase(index);
    this->mapIndex.erase(key);
}

void CObject::setProperty(wchar_t *key, CVariant *v)
{
    auto index = this->mapIndex[key];

    if(index != 0){
        delete this->mapData[index];
    }else{
        index = ++this->autoIndex;
        this->mapIndex[key] = index;
    }
    this->mapData[index] = v;
}
