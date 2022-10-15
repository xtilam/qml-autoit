const path = require('path')
const fs = require('fs')
const color = require('colors')
const config = require('../config')
const { spawn } = require('child_process')

const BUILD_DIR = config.buildTarget
const BUILD_DLL_DIR = path.join(BUILD_DIR, 'qml_dll')
const PRO_FILE_PATH = path.join(__dirname, '../c++/QML_DLL/QML_DLL.pro')

main()

async function main() {
    if (fs.existsSync(BUILD_DIR)) {
        fs.rmSync(BUILD_DIR, { recursive: true })
    }
    fs.mkdirSync(BUILD_DIR, { recursive: true })
    fs.mkdirSync(BUILD_DIR + '/bin', { recursive: true })
    fs.mkdirSync(BUILD_DLL_DIR, { recursive: true })

    await buildDLL(path.join(BUILD_DIR, '/bin/qml.dll')) 
    await compileA3X(path.join(BUILD_DIR, 'bin/libUI.dll'))
    fs.copyFileSync(path.join(__dirname, '../bin/obj.dll'), path.join(BUILD_DIR, '/bin/obj.dll'))
    fs.copyFileSync(path.join(__dirname, '../bin/app.exe'), path.join(BUILD_DIR, 'app.exe'))
    fs.copyFileSync(path.join(config.au3Path, 'AutoIt3_x64.exe'), path.join(BUILD_DIR, '/bin/libMSQL.dll'))

}

async function buildDLL(outDLLPath) {
    const env = {
        PATH: `${config.qt.mingwBin};${config.qt.bin};${process.env.PATH}`,
        ISRELEASE: 1
    }

    await spawnExec('qmake', [
        PRO_FILE_PATH,
        '-spec',
        'win32-g++',
        '"CONFIG-=qml_debug"',
        '"CONFIG+=qtquickcompiler"'
    ],
    {
        cwd: BUILD_DLL_DIR,
        env
    }).promise

    await spawnExec('mingw32-make.exe', [
        '-f',
        path.join(BUILD_DLL_DIR, 'Makefile'),
        'qmake_all',
    ],
    {
        cwd: BUILD_DLL_DIR,
        env
    }).promise

    await spawnExec('mingw32-make.exe', [
        '-j12',
    ],
    {
        cwd: BUILD_DLL_DIR,
        env
    }).promise

    fs.copyFileSync(path.join(BUILD_DLL_DIR, 'release/QML_DLL.dll'), outDLLPath)

    await spawnExec('windeployqt.exe', [
        ...config.windeploytArguments,
        outDLLPath
    ],
    {
        cwd: BUILD_DLL_DIR,
        env
    }).promise

    fs.rmSync(BUILD_DLL_DIR, {recursive: true})

}

async function compileA3X(outFile){
    const option = {
        cwd: BUILD_DIR,
        env: {
            PATH: `${path.join(config.au3Path, 'Aut2Exe')};${process.env.PATH}`
        }
    }

    await spawnExec('Aut2exe_x64', [
        '/in',
        config.mainAu3Path,
        '/out',
        outFile
    ], option).promise
}

function spawnExec(...args) {
    const child = spawn.apply(undefined, [...args])
    child.stdout.on('data', (d) => {
        process.stdout.write(d.toString())
    })
    child.stderr.on('data', (d) => {
        process.stderr.write(d.toString())
    })

    const promise = new Promise((resolve, reject)=>{
        child.on('exit', (code)=>{
            if(code === 0) resolve()
            else reject()
        })
    })

    return {child, promise}
}