QT += quick quickcontrols2 gui

TEMPLATE = lib
DEFINES += QML_DLL_LIBRARY

CONFIG += c++17

SOURCES += \
    Au3.cpp \
    com_object/BaseObject.cpp \
    com_object/CArray.cpp \
    com_object/CObject.cpp \
    com_object/CParams.cpp \
    com_object/CVariant.cpp \
    com_object/CDebugger.cpp \
    com_object/UIObject.cpp \
    exports.cpp

HEADERS += \
    Au3.h \
    QML_DLL_global.h \
    com_object/AllComObject.h \
    com_object/BaseObject.h \
    com_object/CArray.h \
    com_object/CObject.h \
    com_object/CParams.h \
    com_object/CVariant.h \
    com_object/CDebugger.h \
    com_object/UIObject.h \
    exports.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

ISRELEASE = $$(ISRELEASE)

equals(ISRELEASE, 1){
    DEFINES += _RE
    message(release mode)
    RESOURCES += \
        ../../qml/app.qrc
}else{
    DEFINES += _RE_DEV
    message(dev mode)
}

