import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQml 2.12

Item {
	anchors {
		fill: parent
		margins: 40
	}
	Component.onCompleted: {
		register('login', {
			setStatus: function (stt) {
				status.text = stt
			},
			setColorStatus: function (color) {
				status.color = color || 'black'
			},
		})
		register('login.test', {
			test: function (test) {
				console.log('test', JSON.stringify(test, null, ' '))
			},
			
		})
		register('', {
			getUser: function(){
				return {
					"username": usernameInput.text,
					"password": passwordInput.text
				}
			}
		})
	}
	GridLayout {
		id: grid
		columns: 2
		columnSpacing: 10
		width: 300
		anchors.horizontalCenter: parent.horizontalCenter
		anchors.topMargin: 100

		TextField {
			id: usernameInput
			placeholderText: "Username"
			Layout.columnSpan: 2
			Layout.fillWidth: true
		}
		TextField {
			id: passwordInput
			placeholderText: "Password"
			Layout.columnSpan: 2
			Layout.fillWidth: true
			echoMode: TextInput.Password
		}
		Text {
			id: status
			text: ""
			Layout.columnSpan: 2
			font.pixelSize: 18
		}
		Button {
			Layout.fillHeight: true
			Layout.fillWidth: true
			Layout.columnSpan: 2
			text: "Login"
			onClicked: {
				sendSignal('userLogin', 
					{
						"username": usernameInput.text,
						"password": passwordInput.text
					},
				)
			}
		}
	}
}
