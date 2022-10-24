import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQml 2.12

import "main.js" as MainJS

ApplicationWindow{
	visible: true
	id: window
	title: "QML Application"

	Loader{
		anchors.fill: parent
		anchors.margins: 0
		id: _mainLoader
		source: "_Main.qml"
	}

	Component.onCompleted: 
	{
		if(__isDevelopment){
			Qt.createQmlObject(`
				import QtQuick 2.12
					Connections{
					target: au3
					onAu3Reload: {
						g.removeUIMethod()
						_mainLoader.source = ""
						_mainLoader.source = "_Main.qml"
					}
				}`, window)
		}
	}
	onClosing: {
		au3._exit()
	}
}
