#include "lib/qml.au3"

Func onUserLogin($user)
	$ui.setView('test')
	if $user.username = 'admin' and $user.password = 'admin' Then
		$ui.login.setStatus('login success', 'green')
	Else
		$ui.login.setStatus('login error')
	EndIf
EndFunc   ;==>onUserLogin
