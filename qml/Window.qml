import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQml 2.12

ApplicationWindow{
	visible: true
	id: window

	Loader{
		anchors.fill: parent
		anchors.margins: 0
		id: _mainLoader
		source: "Main.qml"
	}

	Component.onCompleted: 
	{
		if(__isDevelopment){
			Qt.createQmlObject(`
							   import QtQuick 2.12
							   Connections{
							   target: au3
							   onAu3Reload: {
							   _mainLoader.source = ""
							   _mainLoader.source = "Main.qml"
							   }
							   }`, window)
		}
	}
	onClosing: {
		au3._exit()
	}
}
