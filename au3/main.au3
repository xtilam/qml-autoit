#include "lib/qml.au3"
#include "login.au3"

main()


Func main()
	QML_SetTheme('Material')

	If Not @Compiled Then
		QML_loadQMLFromFile(@ScriptDir & '\..\qml\Window.qml')
	Else
		QML_loadQMLFromQRC('qrc:/Window.qml')
	EndIf

	While True
		QML_ProcessEvents()
		Sleep(1)
	WEnd
EndFunc   ;==>main

Func onReload()
	;~ $ui.setTitle
EndFunc   ;==>onReload
