const fs = require('fs')
const path = require('path')
const config = require('../config')
const colors = require('colors')

main()

function main() {
    const CONTENTS = getAllContent(config.qt.qtDir)
    const QT_CREATOR_SETTING_PATH = path.join(config.qt.qtDir, 'Tools/QtCreator/share/qtcreator/QtProject/qtcreator')
    const listFilePath = []
    for (const fileName in CONTENTS) {
        const filePath = path.join(QT_CREATOR_SETTING_PATH, `${fileName}.xml`)
        if (!fs.existsSync(filePath)) {
            return console.log(`"${filePath}" not exists`.red)
        }
        listFilePath.push([fileName, filePath])
    }
    console.log('Qt directory valid!'.blue)

    for (const [filename, fullPath] of listFilePath) {
        fs.writeFileSync(fullPath, CONTENTS[filename], { encoding: 'utf-8' })
    }

    console.log('Complete!'.green)
}

function getAllContent(qtPath) {
    return {
        android: `<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE QtCreatorAndroid>
<!-- Written by sdktool , 2022-10-16T09:56:41. 123 -->
<qtcreator>
    <data>
    <variable>MakeExtraSearchDirectory</variable>
    <value type="QString">${qtPath}/Tools/mingw730_64/bin</value>
    </data>
</qtcreator>
`,
        debuggers: `<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE QtCreatorDebuggers>
<!-- Written by sdktool , 2022-10-16T09:56:40. -->
<qtcreator>
 <data>
  <variable>DebuggerItem.0</variable>
  <valuemap type="QVariantMap">
   <valuelist type="QVariantList" key="Abis">
    <value type="QString">x86-windows-msys-pe-64bit</value>
   </valuelist>
   <value type="bool" key="AutoDetected">true</value>
   <value type="QString" key="Binary">${qtPath}/Tools/mingw730_64/bin/gdb.exe</value>
   <value type="QString" key="DisplayName">GNU gdb 8.1 for MinGW 7.3.0 64-bit</value>
   <value type="int" key="EngineType">1</value>
   <value type="QString" key="Id">Debugger.qt.tools.win64_mingw730</value>
  </valuemap>
 </data>
 <data>
  <variable>DebuggerItem.Count</variable>
  <value type="int">1</value>
 </data>
 <data>
  <variable>Version</variable>
  <value type="int">1</value>
 </data>
</qtcreator>
`,
        profiles: `<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE QtCreatorProfiles>
<!-- Written by sdktool , 2022-10-16T09:58:42. -->
<qtcreator>
 <data>
  <variable>Profile.0</variable>
  <valuemap type="QVariantMap">
   <value type="bool" key="PE.Profile.AutoDetected">true</value>
   <valuemap type="QVariantMap" key="PE.Profile.Data">
    <value type="QString" key="Debugger.Information">Debugger.qt.tools.win64_mingw730</value>
    <value type="QString" key="PE.Profile.DeviceType">Desktop</value>
    <valuemap type="QVariantMap" key="PE.Profile.ToolChainsV3">
     <value type="QString" key="C">ProjectExplorer.ToolChain.Mingw:qt.tools.win64_mingw730.gcc</value>
     <value type="QString" key="Cxx">ProjectExplorer.ToolChain.Mingw:qt.tools.win64_mingw730.g++</value>
    </valuemap>
    <value type="QString" key="QtSupport.QtInformation">SDK.qt.qt5.51212.win64_mingw73</value>
   </valuemap>
   <value type="QString" key="PE.Profile.Icon"></value>
   <value type="QString" key="PE.Profile.Id">qt.qt5.51212.win64_mingw73_kit</value>
   <value type="QString" key="PE.Profile.Name">Desktop Qt %{Qt:Version} MinGW 64-bit</value>
   <value type="bool" key="PE.Profile.SDK">true</value>
  </valuemap>
 </data>
 <data>
  <variable>Profile.Count</variable>
  <value type="int">1</value>
 </data>
 <data>
  <variable>Profile.Default</variable>
  <value type="QString">qt.qt5.51212.win64_mingw73_kit</value>
 </data>
 <data>
  <variable>Version</variable>
  <value type="int">1</value>
 </data>
</qtcreator>
`,
        qtversion: `<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE QtCreatorQtVersions>
<!-- Written by sdktool , 2022-10-16T09:58:42. -->
<qtcreator>
 <data>
  <variable>QtVersion.0</variable>
  <valuemap type="QVariantMap">
   <valuelist type="QVariantList" key="Abis"/>
   <value type="int" key="Id">-1</value>
   <value type="QString" key="Name">Qt %{Qt:Version} MinGW 64-bit</value>
   <value type="QString" key="QMakePath">${qtPath}/5.12.12/mingw73_64/bin/qmake.exe</value>
   <value type="QString" key="QtVersion.Type">Qt4ProjectManager.QtVersion.Desktop</value>
   <value type="QString" key="autodetectionSource">SDK.qt.qt5.51212.win64_mingw73</value>
   <value type="bool" key="isAutodetected">true</value>
  </valuemap>
 </data>
 <data>
  <variable>Version</variable>
  <value type="int">1</value>
 </data>
</qtcreator>
`,
        toolchains: `<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE QtCreatorToolChains>
<!-- Written by sdktool , 2022-10-16T09:56:40. -->
<qtcreator>
 <data>
  <variable>ToolChain.0</variable>
  <valuemap type="QVariantMap">
   <value type="QString" key="ProjectExplorer.GccToolChain.Path">${qtPath}/Tools/mingw730_64/bin/gcc.exe</value>
   <valuelist type="QVariantList" key="ProjectExplorer.GccToolChain.SupportedAbis">
    <value type="QString">x86-windows-msys-pe-64bit</value>
   </valuelist>
   <value type="QString" key="ProjectExplorer.GccToolChain.TargetAbi">x86-windows-msys-pe-64bit</value>
   <value type="bool" key="ProjectExplorer.ToolChain.Autodetect">true</value>
   <value type="QString" key="ProjectExplorer.ToolChain.DisplayName">MinGW 7.3.0 64-bit for C</value>
   <value type="QString" key="ProjectExplorer.ToolChain.Id">ProjectExplorer.ToolChain.Mingw:qt.tools.win64_mingw730.gcc</value>
   <value type="QString" key="ProjectExplorer.ToolChain.LanguageV2">C</value>
  </valuemap>
 </data>
 <data>
  <variable>ToolChain.1</variable>
  <valuemap type="QVariantMap">
   <value type="QString" key="ProjectExplorer.GccToolChain.Path">${qtPath}/Tools/mingw730_64/bin/g++.exe</value>
   <valuelist type="QVariantList" key="ProjectExplorer.GccToolChain.SupportedAbis">
    <value type="QString">x86-windows-msys-pe-64bit</value>
   </valuelist>
   <value type="QString" key="ProjectExplorer.GccToolChain.TargetAbi">x86-windows-msys-pe-64bit</value>
   <value type="bool" key="ProjectExplorer.ToolChain.Autodetect">true</value>
   <value type="QString" key="ProjectExplorer.ToolChain.DisplayName">MinGW 7.3.0 64-bit for C++</value>
   <value type="QString" key="ProjectExplorer.ToolChain.Id">ProjectExplorer.ToolChain.Mingw:qt.tools.win64_mingw730.g++</value>
   <value type="QString" key="ProjectExplorer.ToolChain.LanguageV2">Cxx</value>
  </valuemap>
 </data>
 <data>
  <variable>ToolChain.Count</variable>
  <value type="int">2</value>
 </data>
 <data>
  <variable>Version</variable>
  <value type="int">1</value>
 </data>
</qtcreator>
`
    }
}