#include "Au3.h"
#include <QDebug>
#include "exports.h"
#include "com_object/AllComObject.h"

void Au3::registerUI_removeSubObject_Recursive(UIObject *obj)
{
    if(obj->methodId){
        m_ids.push_back(obj->methodId);
        qDebug() << "remove " << obj->methodId;
    }else{
        for(auto &item: obj->mapObject){
            this->registerUI_removeSubObject_Recursive(item.second);
        }
    }
}

Au3::Au3(QObject *parent) : QObject(parent)
{

}

void Au3::pushVariantFromCVariant(QList<QVariant> *arr, CVariant *variant)
{
    QString str;
    QList<QVariant> arrItem;
    CArray * carray;
    double num;
    switch (variant->type) {
    case CVariant::NullValue:
        arr->push_back(QJSValue::UndefinedValue);
        break;
    case CVariant::String:
        variant->getQString(&str);
        arr->push_back(str);
        break;
    case CVariant::Boolean:
        arr->push_back(((bool)variant->ptr) ? true: false);
        break;
    case CVariant::Number:
        variant->getNumber(&num);
        arr->push_back(num);
        break;
    case CVariant::Array:
        variant->getArray(&carray);
        for(auto &item: carray->items){
            Au3::pushVariantFromCVariant(&arrItem, item);
        }
        arr->push_back(arrItem);
        break;
    case CVariant::Object:
    {
        CObject * obj;
        variant->getObject(&obj);
        QMap<QString, QVariant> map;
        for(auto &i: obj->mapIndex){
            QString key = QString::fromStdWString(i.first);
            setPropertyVariantFromObject(&map, &key, obj->mapData[i.second]);
        }
        arr->push_back(map);
    }
        break;
    }
}

void Au3::setPropertyVariantFromObject(QMap<QString, QVariant> *map, QString* key, CVariant *variant)
{
    switch (variant->type) {
    case CVariant::NullValue:
        (*map)[*key] = (QJSValue::UndefinedValue);
        break;
    case CVariant::String:
    {
        QString str;
        variant->getQString(&str);
        (*map)[*key] = str;
    }
        break;
    case CVariant::Boolean:
        (*map)[*key] = ((bool)variant->ptr) ? true: false;
        break;
    case CVariant::Number:
    {
        double num;
        variant->getNumber(&num);
        (*map)[*key] = num;
    }

        break;
    case CVariant::Array:
    {
        QList<QVariant> list;
        CArray * arr;
        variant->getArray(&arr);
        for(auto &item: arr->items){
            Au3::pushVariantFromCVariant(&list, item);
        }
        (*map)[*key] = list;
    }
        break;
    case CVariant::Object:
    {
        QMap<QString, QVariant> subMap;
        CObject * obj;
        variant->getObject(&obj);
        for(auto &i: obj->mapIndex){
            QString key = QString::fromStdWString(i.first);
            setPropertyVariantFromObject(&subMap, &key, obj->mapData[i.second]);
        }
        (*map)[*key] = subMap;
        break;
    }
    }
}

void Au3::setDataCArrayFromListVariant(CArray *arr, QList<QVariant> *listItem)
{
    CVariant * variant = nullptr;
    for(auto &item: *listItem){
        variant = new CVariant;
        if(!item.isNull()){
            switch(item.type()){
            case QVariant::String:
            {
                auto str = item.toString();
                if(str.isEmpty()) break;
                variant->setValueFromQStringRef(str);
                break;
            }
            case QVariant::Int:
                variant->setValueFromNumber(item.toInt());
                break;
            case QVariant::Double:
                variant->setValueFromNumber(item.toDouble());
                break;
            case QVariant::Map:
            {
                auto subMap = item.toMap();
                CObject * subObj = BaseObject::init<CObject>();
                setDataCObjectFromMapVariant(subObj, &subMap);
                variant->setValueFromObject(subObj);
                break;
            }

            case QVariant::List:
            {
                CArray * subArr = BaseObject::init<CArray>();
                auto subList = item.toList();
                variant->setValueFromObject(subArr);
                setDataCArrayFromListVariant(subArr, &subList);
                break;
            }
            default: break;
            }
        }

        arr->push({variant});
    }
}

void Au3::setDataCObjectFromMapVariant(CObject *obj, QMap<QString, QVariant> *mapItem)
{
    CVariant * variant = nullptr;
    for (QMap<QString, QVariant>::iterator i = mapItem->begin(); i != mapItem->end(); ++i){
        auto item = &i.value();
        variant = new CVariant;
        if(!item->isNull()){
            switch(item->type()){
            case QVariant::String:
            {
                auto str = item->toString();
                if(str.isEmpty()) break;
                variant->setValueFromQString(str);
                break;
            }
                break;
            case QVariant::Int:
                variant->setValueFromNumber(item->toInt());
                break;
            case QVariant::Double:
                variant->setValueFromNumber(item->toDouble());
                break;
            case QVariant::Map:
            {
                auto subMap = item->toMap();
                CObject * subObj = BaseObject::init<CObject>();
                setDataCObjectFromMapVariant(subObj, &subMap);
                variant->setValueFromObject(subObj);
                break;
            }
            case QVariant::List:
            {
                CArray * subArr = BaseObject::init<CArray>();
                auto subList = item->toList();
                setDataCArrayFromListVariant(subArr, &subList);
                variant->setValueFromObject(subArr);
                break;
            }
            default:break;
            }
        }
        auto key = i.key();
        obj->setProperty((wchar_t *)key.toStdWString().c_str(), variant);
    }
}

void Au3::registerUI(QJSValue listNames, QString name, int id)
{
    auto list = listNames.toVariant().toList();

    UIObject * obj = ui;
    UIObject * itemObj = nullptr;

    for (auto &item: list) {
        auto key = item.toString().toStdWString();
        if(key.empty()) continue;
        obj = obj->add((wchar_t *)key.c_str());
    }
    itemObj = obj->add((wchar_t *)name.toStdWString().c_str());
    itemObj->setMethodID(id);
}

QList<QVariant> Au3::getUIData()
{
    QList<QVariant> result;

    if(!inputs) return result;

    int length = inputs->length();
    CVariant variant;

    for(int i=0; i<length; i++){
        inputs->nextToCVariant(&variant);
        Au3::pushVariantFromCVariant(&result, &variant);
    }

    return result;
}

void Au3::sendSignal(QString signal, QJSValue data)
{
    if(data.isArray()){
        auto arr = BaseObject::init<CArray>();
        auto listVariant = data.toVariant().toList();
        setDataCArrayFromListVariant(arr, &listVariant);
        Au3Callback::handleSignal(signal.toStdString().c_str(), arr->comobject);
    }
}

void Au3::_exit()
{
    exit(0);
}
