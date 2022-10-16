const fs = require('fs')
const path = require('path')
const colors = require('colors')
const { spawnSync, spawn, execSync } = require('child_process')
const nodemon = require('nodemon')
const config = require('../config')

main()

async function main() {
    let au3Process;
    let isStartApp = false
    const autoIt3WrapperAu3Path = path.join(config.au3Path, 'SciTE/AutoIt3Wrapper/AutoIt3Wrapper.au3')


    watcherFiles()
    spawnExec('node', [path.join(__dirname, 'watch-qml.js')])
    watchAU3()

    function watchAU3() {
        runAu3()
        const mon = nodemon({
            exec: ` `,
            watch: path.join(config.mainAu3Path, '..'),
            ext: 'au3',
            stdout: true,
            stdin: true
        })
        mon.on("restart", function (files) {
            runAu3()
        })
    }

    function watcherFiles() {
        for (const [source, target] of config.watchers) {
            watchOtherBuild(source, target)
        }
    }

    function runAu3() {
        if (isStartApp) return
        isStartApp = true

        setTimeout(() => {
            if (au3Process) {
                try {
                    execSync(`taskkill /f /pid ${au3Process.pid}`)
                } catch (error) { }
                console.log('=> restart au3 app\n'.green.bold)
            } else {
                console.log('=> start au3 app\n'.green.bold)
            }

            
            let currentProcess = spawnExec(path.join(config.au3Path, 'AutoIt3_x64.exe'), [
                autoIt3WrapperAu3Path,
                '/run',
                '/x64',
                '/prod',
                '/ErrorStdOut',
                '/in',
                `${config.mainAu3Path}`
            ],
                {
                    env: {
                        PATH: `${config.qt.bin};${config.qt.plugins};${path.join(__dirname, '../bin')};${process.env.PATH}`,
                        QT_PLUGIN_PATH : config.qt.plugins,
                        QML_IMPORT_PATH: path.join(path.dirname(config.qt.bin), 'qml'),
                        QML2_IMPORT_PATH: path.join(path.dirname(config.qt.bin), 'qml'),
                        // FORCE_COLOR: true
                    },
                }
            )

            currentProcess.on('exit', (exitCode) => {
                if (au3Process !== currentProcess) return
                console.log('=> exit au3 app\n'.red.bold)
            })

            au3Process = currentProcess

            isStartApp = false
        }, 500)
    }

    async function watchOtherBuild(source, dest, timeCopy = 3000) {

        const dirDest = path.dirname(dest)
        let oldTimeStartCopy = 0

        try {
            await copyFile(false)
        } catch (error) {
            console.log(`copy error[ ${error} ]: ${source} => ${dest}`)
        }

        fs.watchFile(source, { interval: 100 }, (cur, pre) => {
            console.log('file change', source)
            if (au3Process) {
                try {
                    execSync(`taskkill /f /pid ${au3Process.pid}`)
                } catch (error) { }
                console.log('=> restart au3 app\n'.green.bold)
            } else {
                console.log('=> start au3 app\n'.green.bold)
            }
            if (cur.size) return copyFile().then(runAu3).catch(err => console.log(`copy error[ ${err} ]: ${source} => ${dest}`))
        })

        async function copyFile(checkExists = true) {
            return new Promise((resolve, reject) => {
                if (checkExists) {
                    if (!fs.existsSync(source)) return resolve()
                } else if (oldTimeStartCopy) {
                    return resolve()
                }


                if (!fs.existsSync(dirDest)) fs.mkdirSync(dirDest, { recursive: true })
                if (!fs.lstatSync(dirDest).isDirectory()) {
                    fs.rmSync(dirDest)
                    fs.mkdirSync(dirDest, { recursive: true })
                }
                oldTimeStartCopy = new Date().getTime()

                const _interval = setInterval(() => {
                    if (!checkValidTime()) {
                        clearTimeout(_interval)
                        console.log('checkValidTime', checkValidTime())
                        reject('timeout error')
                    }

                    try {
                        fs.copyFileSync(source, dest)
                        clearTimeout(_interval)
                        resolve()
                    } catch (error) { }
                }, 300)

                function checkValidTime() {
                    return (new Date().getTime() - oldTimeStartCopy) < timeCopy
                }
            })
        }
    }
}

function spawnExec(...args) {
    const child = spawn.apply(undefined, [...args])
    child.stdout.on('data', (d) => {
        process.stdout.write(d.toString())
    })
    child.stderr.on('data', (d) => {
        process.stderr.write(d.toString())
    })
    return child
}