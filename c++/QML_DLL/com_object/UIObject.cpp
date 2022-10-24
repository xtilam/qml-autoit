#include "UIObject.h"
#include "exports.h"
#include "Au3.h"
#include "AllComObject.h"
#include <QMetaObject>

UIObject *UIObject::add(wchar_t *key)
{
    if(this->methodId){
        au3->m_ids.push_back(this->methodId);
        this->methodId = 0;
    }

    auto _index = this->mapID[key];
    UIObject * obj = nullptr;

    if(_index){
        obj = this->mapObject[_index];
    }else{
        obj = BaseObject::init<UIObject>();
        obj->addRef();
        this->mapID[key] = this->autoIndex;
        this->mapObject[this->autoIndex] = obj;
        ++autoIndex;
    }

    return obj;
}

long UIObject::notPrototypeGetID(wchar_t *key)
{
    return this->mapID[key];
}

void UIObject::notPrototypeExecute(long key, FunctionInput *params)
{

    UIObject * obj = mapObject[key];
    if(!obj) return;

    if(obj->methodId == 0){
        params->setResultObject(obj);
    }else{
        au3->inputs = params;
        auto result = engine->evaluate(
                    QString::fromStdString("g.callUIMethod(")
                    .append(QString::number(obj->methodId))
                    .append(")")
                    ).toVariant();

        if(!result.isNull()){
            switch (result.type()) {
            case QVariant::String:
            {
                auto str = result.toString();
                if(str.isEmpty()) break;
                params->setResultString((wchar_t *)str.toStdWString().c_str());
                break;
            }
                break;
            case QVariant::Int:
                params->setResultNumber(result.toInt());
                break;
            case QVariant::Double:
                params->setResultNumber(result.toDouble());
                break;
            case QVariant::Map:
            {
                auto subMap = result.toMap();
                CObject * subObj = BaseObject::init<CObject>();
                Au3::setDataCObjectFromMapVariant(subObj, &subMap);
                params->setResultObject(subObj);
                break;
            }
            case QVariant::List:
            {
                CArray * subArr = BaseObject::init<CArray>();
                auto subList = result.toList();
                Au3::setDataCArrayFromListVariant(subArr, &subList);
                params->setResultObject((BaseObject *)subArr);
                break;
            }
            }
        }
    }
}

void UIObject::onDelete()
{
    this->clear();
    delete this;
}

void UIObject::clear()
{
    for(auto &obj: this->mapObject){
        BaseObject::deleteObject(obj.second->comobject);
    }
    if(this->methodId){
        au3->m_ids.push_back(this->methodId);
    }
    this->mapObject = {};
    this->mapID = {};
    this->autoIndex = 1;
    this->methodId = 0;
}

void UIObject::setMethodID(int id)
{
    this->clear();
    if(this->methodId){
        au3->m_ids.push_back(this->methodId);
    }
    this->methodId = id;
}
