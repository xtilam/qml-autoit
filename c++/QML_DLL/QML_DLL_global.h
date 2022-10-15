#ifndef QML_DLL_GLOBAL_H
#define QML_DLL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QML_DLL_LIBRARY)
#  define QML_DLL_EXPORT Q_DECL_EXPORT
#else
#  define QML_DLL_EXPORT Q_DECL_IMPORT
#endif

#endif // QML_DLL_GLOBAL_H
