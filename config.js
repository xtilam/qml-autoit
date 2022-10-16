const path = require('path')

const qtDir = 'E:/Qtcc/Qt5.12.12'

const config = {
    au3Path: 'C:/Program Files (x86)/AutoIt3',
    autoItBuildName: 'FoxApp.exe',
    buildTarget: path.join(__dirname, './build'),
    mainAu3Path: path.join(__dirname, 'au3/main.au3'),
    qt: {
        bin: path.join(qtDir, '/5.12.12/mingw73_64/bin'),
        plugins: path.join(qtDir, '/5.12.12/mingw73_64/plugins'),
        mingwBin: path.join(qtDir, '/Tools/mingw730_64/bin'),
    },
    watchers: [
        // [
        //     path.join(__dirname, 'c++/_release/release/QML_DLL.dll'),
        //     path.join(__dirname, 'bin/qml.dll')
        // ],
        // [
        //     path.join(__dirname, 'c++/ComObj/x64/Release/ComObj.dll'),
        //     path.join(__dirname, 'bin/obj.dll')
        // ],
        // [
        //     path.join(__dirname, 'c++/Au3Start/x64/Release/Au3Start.exe'),
        //     path.join(__dirname, 'bin/app.exe')
        // ],
    ],
    windeploytArguments: [
        // '--no-patchqt',
        // '--no-plugins',
        '--no-system-d3d-compiler',
        '--no-translations',
        '--no-opengl',
        '--no-opengl-sw',
        '--no-widgets',
        '--no-angle',
        '--qmldir',
        path.join(__dirname, 'qml'),
        '--release',
        '--no-qmltooling',
    ],
}

module.exports = config