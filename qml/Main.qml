import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQml 2.12

import "main.js" as MainJS

Rectangle {
	id: root
	width: 600
	height: 400
	property string view: "Login"
		Pane{
			padding: 0
			anchors{
				fill: parent
				left: parent.left
			}
			Material.accent: Material.Blue
			Loader{
				id: mainLoader
				anchors.fill: parent
				source: view + ".qml"
			}
		}

		Component.onCompleted: {
			g.root = root
			g.mainLoader = mainLoader
			window.title = 'QML Application'
			window.width = root.width
			window.height = root.height
			if (__isDevelopment)
			{
				window.y =40
				window.x = 1320
			}

			register('', {
				setWindow: function({x, y, width, height, title}){
					if (Number.isNumber(x))
					{
						window.x = x
					}
					if (Number.isNumber(y))
					{
						window.y = y
					}
					if (Number.isNumber(width))
					{
						window.width = width
					}
					if (Number.isNumber(height))
					{
						window.height = height
					}
					if (typeof title === 'string')
					{
						window.title = title;
					}
				},
				setView(view){
					if (typeof view !== 'string') return;
					root.view = view
				},
				getTitle(){
					return window.title
				}
		})
	}
}
