import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

Item {
	Column{
		Button{
			text: "This is main view"
		}
		Button{
			text: "Logout"
			onClicked: {
				call("userLogout")
			}
		}
	}
}
