
#include-once

Global $QML_DLL = 0
Global $OBJ_DLL = 0

QML_loadDLL()


Opt("TrayIconHide", 1)

Func QML_loadDLL()
	If $QML_DLL = 0 Then
		$OBJ_DLL = DllOpen('obj.dll')
		$QML_DLL = DllOpen('qml.dll')

		If $OBJ_DLL = -1 Or $QML_DLL = -1 Then
			ConsoleWrite('Cannot load DLL' & @CRLF)
			Exit
		EndIf

		If Not @Compiled Then
			QML_enableDebug()
		EndIf

		Local $methods = DllCall($OBJ_DLL, 'ptr', 'getAllMethods')[0]
		DllCall($QML_DLL, 'none', 'setComMethods', 'ptr', $methods)
		Global Const $console = DllCall($QML_DLL, 'idispatch', 'getDebugger')[0]
		Global Const $ui = DllCall($QML_DLL, 'idispatch', 'getUI')[0]

		Local $configMethods = DllStructCreate('ptr methods[3]')
		DllStructSetData($configMethods, 'methods', DllCallbackGetPtr(DllCallbackRegister(QML_mainHandleSignal, 'none', 'str;idispatch')), 1)		
		if IsFunc(Execute('onReload')) Then 
			DllStructSetData($configMethods, 'methods', DllCallbackGetPtr(DllCallbackRegister('onReload', 'none', '')), 2)
		EndIf
		DllCall($QML_DLL, 'none', 'configMethods', 'ptr', DllStructGetPtr($configMethods))
	EndIf
EndFunc   ;==>QML_loadDLL

;~ For dev mode
Func QML_loadQMLFromFile($file)
	DllCall($QML_DLL, 'none', 'qmlLoadFile', 'wstr', $file)
	QML_ProcessEvents()
EndFunc   ;==>QML_loadQMLFromFile
;~ For release mode
Func QML_loadQMLFromQRC($file)
	DllCall($QML_DLL, 'none', 'qmlLoadQRC', 'wstr', $file)
	QML_ProcessEvents()
EndFunc   ;==>QML_loadQMLFromQRC

Func QML_mainHandleSignal($func, $p)

	Local $length = $p.length - 1
	Local $start = ''
	$strExecute = 'on' & $func & '('
	
	For $i = 0 To $length Step +1
		$strExecute &= $start & '$p.get(' & $i & ')'
		$start = ', '
	Next
	$strExecute &= ')'

	Execute($strExecute)

EndFunc

Func QML_enableDebug()
	DllCall($QML_DLL, 'none', 'enableDebug')
EndFunc   ;==>QML_enableDebug

Func QML_newObject()
	return DllCall($QML_DLL, 'idispatch', 'newObject')[0]
EndFunc

Func QML_newArray()
	return DllCall($QML_DLL, 'idispatch', 'newArray')[0]
EndFunc

Func QML_SetTheme($theme)
	return DllCall($QML_DLL, 'none', 'setTheme', 'wstr', $theme)[0]
EndFunc

Func QML_ProcessEvents()
	DllCall($QML_DLL, 'none', 'execLoopEvent')
EndFunc
