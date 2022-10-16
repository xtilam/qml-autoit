#include "lib/qml.au3"
#include "login.au3"

main()

Func main()
	QML_SetTheme('Material')

	If Not @Compiled Then
		QML_loadQMLFromFile(@ScriptDir & '/../qml/_Window.qml')
	Else
		QML_loadQMLFromQRC('qrc:/_Window.qml')
		ConsoleWrite('from qml dll 123' & @CRLF)
	EndIf

	$ui.setTitle('Login')

	While True
		QML_ProcessEvents()
		Sleep(1)
	WEnd
EndFunc   ;==>main

Func onReload()
	;~ $console.Log('on reload called')
	;~ $ui.showData( _
	;~ 	QML_newObject() _
	;~ 		.set('name', 'cuong toi') _
	;~ 		.set('test', 'cuong 12') _
	;~ 		.set('data', QML_newArray() _ 
	;~ 			.push(1,2,3) _
	;~ 		) _
	;~ )
EndFunc   ;==>onReload
