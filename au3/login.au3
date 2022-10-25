#include "lib/qml.au3"

Func userLogin($user)
	if $user.username = 'admin' and $user.password = 'admin' Then
		$ui.setView('MainView')
		$ui.setTitle('Welcome')
	Else
		$ui.login.setStatus('Login Failed')
		$ui.login.setColorStatus('red')
	EndIf
EndFunc   ;==>onUserLogin

Func userLogout()
	$ui.setTitle('Login')
	$ui.setView('Login')
EndFunc