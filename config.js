const path = require('path')

const config = {
    au3Path: 'C:/Program Files (x86)/AutoIt3',
    qt: {
        bin: 'E:/Qt/Qt5.12.12/5.12.12/mingw73_64/bin',
        mingwBin: 'E:/Qt/Qt5.12.12/Tools/mingw730_64/bin',
    },
    mainAu3Path: path.join(__dirname, 'au3/main.au3'),
    watchers: [
        [
            path.join(__dirname, 'c++/ComObj/x64/Release/ComObj.dll'),
            path.join(__dirname, 'bin/obj.dll')
        ],
        [
            path.join(__dirname, 'c++/_release/release/QML_DLL.dll'),
            path.join(__dirname, 'bin/qml.dll')
        ],
        [
            path.join(__dirname, 'c++/Au3Start/x64/Release/Au3Start.exe'),
            path.join(__dirname, 'bin/app.exe')
        ],
        [
            path.join(__dirname, 'c++/Au3Start/x64/Release/Au3Start.exe'),
            path.join(__dirname, 'build/app.exe')
        ],
    ],
    buildTarget: path.join(__dirname, './build'),
    windeploytArguments: [
        '--no-patchqt',
        '--no-plugins',
        '--no-system-d3d-compiler',
        '--no-translations',
        '--no-opengl',
        '--no-opengl-sw',
        '--no-widgets',
        '--no-angle',
    ],
    autoItBuildName: 'FoxApp.exe'
}

module.exports = config