#include "exports.h"
#include <windows.h>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <thread>
#include <Au3.h>
#include <QQmlContext>
#include "com_object/AllComObject.h"
#include <QTextCodec>
#include <QQuickStyle>
#include <QIcon>


using namespace std;

bool isDevLoad = false;
bool isRestart = true;
int reloadCounted = 0;
const int maxReload = 20;
int argc = 0;
QDir pathReload;
thread watchReloadSignalThread;
QString reloadFilePath;


void enableDebug(){
    CDebugger::isEnableDebug = true;
    AllocConsole();

    COORD coordInfo;
    coordInfo.X = 130;
    coordInfo.Y = 9000;

    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coordInfo);
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_QUICK_EDIT_MODE | ENABLE_EXTENDED_FLAGS);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
}

void qmlLoadFile(LPCWSTR qmlFilePath)
{
    isDevLoad = true;
    isRestart = true;
    reloadCounted = maxReload;

    reloadFilePath = QString::fromStdWString(qmlFilePath);

    QFileInfo resourceFileInfo(reloadFilePath);
    QDir resourceFolder = resourceFileInfo.dir();

    if (!resourceFileInfo.exists()) {
        qDebug() << "qml resource not exists";
        exit(1);
    }

    pathReload = QString::fromStdString(resourceFileInfo.absolutePath().toStdString()).append("/_scriptReload.reload");
    pathReload.removeRecursively();
}

void qmlLoadQRC(LPCWSTR qmlFilePath)
{
    isDevLoad = false;
    app = new QGuiApplication(argc, nullptr);
    app->setWindowIcon(QIcon(":/app.ico"));
    engine = new QQmlApplicationEngine();
    engine->evaluate("var isDev = false; var g = this;");
    engine->rootContext()->setContextProperty("au3", au3);
    engine->load(QString::fromStdWString(qmlFilePath));
}

void setComMethods(void **methods)
{
    BaseObject::initComObjectMethod = (BaseObject::TInitComObjectType)methods[0];
    BaseObject::getBaseObject = (BaseObject::TGetCObject)methods[1];
    BaseObject::allocString = (BaseObject::TAllocString)methods[2];
    BaseObject::deleteObject = (BaseObject::TDeleteMethod)methods[3];
    BaseObject::addRefObject = (BaseObject::TAddRef)methods[4];
    BaseObject::releaseRefObject = (BaseObject::TRelaseRef)methods[5];
    BaseObject::freeString = (BaseObject::TFreeString)methods[6];
    au3 = new Au3();
}

void configMethods(void **methods)
{
    Au3Callback::handleSignal = (Au3Callback::CallbackType::handleSignal) methods[0];
    Au3Callback::onReload = (Au3Callback::CallbackType::onReload) methods[1];
}

LPDISPATCH newObject()
{
    auto obj = BaseObject::init<CObject>();
    return obj->comobject;
}

LPDISPATCH newArray()
{
    return BaseObject::init<CArray>()->comobject;
}

LPDISPATCH getDebugger()
{
    auto debugger = BaseObject::init<CDebugger>();
    return debugger->comobject;
}

void setTheme(LPCWSTR theme)
{
    QQuickStyle::setStyle(QString::fromStdWString(theme));
}

void execLoopEvent()
{
    if(!isDevLoad) return app->processEvents();
    if(!isRestart) return app->processEvents();

    isRestart = false;

    if(reloadCounted >= maxReload){
        if(engine != nullptr) {
            delete engine;
            delete app;
        }
        app = new QGuiApplication(argc, nullptr);
        engine = new QQmlApplicationEngine();
        engine->rootContext()->setContextProperty("au3", au3);
        engine->evaluate("var __isDevelopment = true; var g = this;");
        reloadCounted = 0;
        engine->load(reloadFilePath);
    }

    watchReloadSignalThread = thread([](QDir* pathReload) {
        while (!pathReload->exists()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        pathReload->removeRecursively();
        isRestart = true;
        }, &pathReload
    );

    watchReloadSignalThread.detach();
    engine->clearComponentCache();
    engine->collectGarbage();
    ui->clear();
    au3->au3Reload();
    if(Au3Callback::onReload) {
        Au3Callback::onReload();
    }
    reloadCounted++;
    return app->processEvents();
}

LPDISPATCH getUI()
{
    if(!ui){
        ui = BaseObject::init<UIObject>();
    }
    return ui->comobject;
}
