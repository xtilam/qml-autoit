const path = require('path')
const os = require('os')
const { readFileSync, existsSync } = require('fs')
const color = require('colors')
const { exit } = require('process')

color.enable()


const qtDir = getQtDir()

const qtPath = {
    bin: path.join(qtDir, '/5.12.12/mingw73_64/bin'),
    plugins: path.join(qtDir, '/5.12.12/mingw73_64/plugins'),
    mingwBin: path.join(qtDir, '/Tools/mingw730_64/bin'),
    qtDir,
}

const au3DevEnv = {
    PATH: `${qtPath.bin};${path.join(__dirname, './bin')};${process.env.PATH}`,
    QT_PLUGIN_PATH: qtPath.plugins,
    QML_IMPORT_PATH: path.join(path.dirname(qtPath.bin), 'qml'),
    QML2_IMPORT_PATH: path.join(path.dirname(qtPath.bin), 'qml'),
}

const config = {
    au3Path: 'C:/Program Files (x86)/AutoIt3',
    autoItBuildName: 'FoxApp.exe',
    buildTarget: path.join(__dirname, './build'),
    mainAu3Path: path.join(__dirname, 'au3/main.au3'),
    qt: qtPath,
    au3DevEnv,
    watchers: [
        [
            path.join(__dirname, 'c++/_release/release/QML_DLL.dll'),
            path.join(__dirname, 'bin/qml.dll')
        ],
        [
            path.join(__dirname, 'c++/ComObj/x64/Release/ComObj.dll'),
            path.join(__dirname, 'bin/obj.dll')
        ],
        [
            path.join(__dirname, 'c++/Au3Start/x64/Release/Au3Start.exe'),
            path.join(__dirname, 'bin/app.exe')
        ],
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

function getQtDir() {
    const qt_install_data = "C:\\ProgramData\\_qt_install_dir.txt"
    if (!existsSync(qt_install_data)) {
        console.log('Qt chưa được cài đặt')
        exit(1)
    }
    const qtPath = path.join(readFileSync(qt_install_data, { encoding: 'utf-8' }))
    if(!qtPath){
        console.log('Qt chưa được cài đặt')
        exit(1)
    }
    return qtPath
}

module.exports = config