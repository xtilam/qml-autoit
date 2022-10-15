#ifndef EXPORTS_H
#define EXPORTS_H

#include "QML_DLL_global.h"

#include <QDebug>
#include <windows.h>
#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>

class UIObject;
class Au3;

inline Au3 * au3;
inline UIObject * ui;
inline QGuiApplication* app = nullptr;
inline QQmlApplicationEngine* engine = nullptr;

extern "C" QML_DLL_EXPORT void __stdcall enableDebug();
extern "C" QML_DLL_EXPORT void __stdcall qmlLoadFile(LPCWSTR qmlFilePath);
extern "C" QML_DLL_EXPORT void __stdcall qmlLoadQRC(LPCWSTR qmlFilePath);
extern "C" QML_DLL_EXPORT void __stdcall configMethods(void ** methods);
extern "C" QML_DLL_EXPORT LPDISPATCH __stdcall newObject();
extern "C" QML_DLL_EXPORT LPDISPATCH __stdcall newArray();
extern "C" QML_DLL_EXPORT LPDISPATCH __stdcall getDebugger();
extern "C" QML_DLL_EXPORT void __stdcall setTheme(LPCWSTR theme);
extern "C" QML_DLL_EXPORT LPDISPATCH __stdcall getUI();
extern "C" QML_DLL_EXPORT void __stdcall execLoopEvent();
extern "C" QML_DLL_EXPORT void __stdcall setComMethods(void ** method);

namespace Au3Callback{
namespace CallbackType{
    typedef void (*handleSignal)(LPCSTR signal, LPDISPATCH params);
    typedef void (*onReload)();
}
    inline CallbackType::handleSignal handleSignal = nullptr;
    inline CallbackType::onReload onReload = nullptr;
}

#endif // EXPORTS_H
