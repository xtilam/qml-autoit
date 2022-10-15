#ifndef AU3_H
#define AU3_H

#include <QObject>
#include <QJSValue>
#include <QVariant>
#include <QMap>

class UIObject;
class FunctionInput;
class CVariant;
class CArray;
class CObject;

class Au3 : public QObject
{
    Q_OBJECT
public:
    explicit Au3(QObject *parent = nullptr);

    FunctionInput * inputs;

    static void pushVariantFromCVariant(QList<QVariant> * arr, CVariant * variant);
    static void setPropertyVariantFromObject(QMap<QString, QVariant> * map, QString * key, CVariant * obj);
    static void setDataCArrayFromListVariant(CArray * arr, QList<QVariant> * listItem);
    static void setDataCObjectFromMapVariant(CObject * obj, QMap<QString, QVariant> * mapItem);
public slots:

    void registerUI(QJSValue listNames, QString name, int id);
    QList<QVariant> getUIData();
    void sendSignal(QString signal, QJSValue data);
    void _exit();
signals :
    void au3Reload();
};

#endif // AU3_H
