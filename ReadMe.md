# Files:
- Qt5.12.12.rar: https://drive.google.com/file/d/1FRw7GB-ndbP-0iGVHTy-u4hDfetYxJWh/view 
- Source code: https://github.com/xtilam/qml-autoit

# Môi trường
- Nodejs v16 trở lên
- AutoIt3 + Scite
- VSCode IDE (cho tiện)
- Qt5.12.12.rar => giải nén chạy file install.bat

# Các lệnh npm cơ bản
```text\
npm run dev => chạy môi trường dev, auto reload .au3 và .qml
npm run build => build app
```

# Source Code:
```text
- au3: Source code AU3
- qml: Source qml -> cho ui
- dev: chứa các script build và chạy app trong môi trường phát triển
- config.js: chứa cấu hình
    + qtDir: Path Qt5.12.12.rar giải nén (đường dẫn đến trước thư mục Tools)
    + au3DevEnv: biến môi trường khi chạy script autoit
    + config
        + au3Path: đường dẫn autoit cài trong máy
        + autoItBuildName: tên process khi chạy build ra
        + buildTarget: đường dẫn thư mục build
        + mainAu3Path: đường dẫn file autoit script chính sẽ chạy
        + watchers: khi chạy trong môi trường dev sẽ tự động theo dõi và copy các file vào thư mục chỉ định, sẽ reload app nếu build thay đổi
            - qml.dll: dll chính dùng trong môi trường phát triển
            - obj.dll: mingw64 trong Qt ko compile đc Com Object nên phải buid riêng bằng msvc
            - app.exe: dùng trong file build, sẽ khởi động autoit từ a3x đã compile, thay thế file .bat
        + windeploytArguments: các tham số cấu hình khi deploy qt, xem thêm (Qt for Windows - Deployment)
```


# Sử dụng cơ bản
## Object cơ bản
```autoit
#include "lib/qml.au3"

Func test()
    ;Tạo một object
    Local $human = QML_newObject()

    ; đặt thuộc tính cho object
    $human.set('name', 'cuong')
    $human.set('age', 14)

    ;có thể gọi như thế này
    $human.set('email', 'test@gmail.com') _
        .set('address', '48 Bùi Thị Xuân')
    
    ;hiển thị object trong console, hàm không giới hạn tham số
    $console.log($human)

    ;lấy thuộc tính cho object
    Local $name = $human.name
    Local $address = $human.get('address')

    ;xóa thuộc tính cho object
    $human.delete('name')

    $console.log($human, $human.name)
EndFunc
```
## Array cơ bản
```autoit
#include "lib/qml.au3"

Func test()
    ;Tạo một object
    Local $arr = QML_newAray()

    ; thêm giá trị vào trong array
    $arr.push('name', 'cuong')
    $arr.push('123', 44)

    ; lấy độ dài mảng
    Local $length = $arr.length()
    ; lặp mảng
    For $i=0 To $length - 1 Step + 1
        $console.log($arr.get($i))
    Next    
EndFunc
```

## Gọi một hàm trong AutoIt từ qml
Từ file .qml
```qml
Button {
    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.columnSpan: 2
    text: "Login"
    onClicked: {
        // Gửi signal cho AutoIt
        // Sẽ gọi hàm userLogin trong Autoit
        call('userLogin', {
            "username": usernameInput.text,
            "password": passwordInput.text
        })
    }
}
```
Từ file .au3
```autoit
;Hàm này sẽ được gọi khi hàm "call" trong QML được gọi
Func userLogin($user)
    Local $username = $user.username
    Local $password = $user.password
    $console.Log('user', $user)
EndFunc
```
## Đăng kí một function từ qml sang cho autoit gọi
Từ file .qml
```qml
Component.onCompleted: {
    register('login', {
        // gọi trong autoit: $ui.login.setStatus
        setStatus: function (stt) {
            status.text = stt
        },
        // gọi trong autoit: $ui.login.setColorStatus
        setColorStatus: function (color) {
            status.color = color || 'black'
        },
    })
    register('', {
        // gọi trong autoit: $ui.getUser
        getUser: function(){
            return {
                "username": usernameInput.text,
                "password": passwordInput.text
            }
        }
    })
    register('login.test', {
        // gọi trong autoit: $ui.login.test.test
        test: function (test) {
            console.log('test', JSON.stringify(test, null, ' '))
        },     
    })
}
```
Từ file .au3
```autoit
Local $objInput = QML_newObject() _ 
    .set('name', 'cuong') _
    .set('age', 12) _

Local $user = $ui.getUser()
$console.Log($user)
$ui.login.setStatus('login Success')
$ui.login.setColorStatus('green')
$ui.login.test.test($objInput)
```

## Set Theme QML

```autoit
;https://doc.qt.io/qt-5/qtquickcontrols2-styles.html
QML_SetTheme('Material')
```