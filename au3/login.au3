#include "lib/qml.au3"

Func onUserLogin($user)
	if $user.username = 'admin' and $user.password = 'admin' Then
		$ui.setView('MainView')
		$ui.setTitle('Welcome')
	Else
		$ui.login.setStatus('Login Failed')
		$ui.login.setColorStatus('red')
	EndIf
EndFunc   ;==>onUserLogin

Func onUserLogout()
	$ui.setTitle('Login')
	$ui.setView('Login')
EndFunc