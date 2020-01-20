import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.12

Item{
    id: imgControl

    property string picPath: ""

    clip: true

    Button {
        id: imgControlButton
        //        flat: true
        anchors.centerIn: parent
        text: qsTr("选择图片")

        FileDialog {
            id:fds
            title: "选择文件"
            folder: shortcuts.desktop
            selectExisting: true
            selectFolder: false
            selectMultiple: false
            nameFilters: ["png文件 (*.png)"]
            onAccepted: {
                picPath = fds.fileUrl;
                imgControlButton.visible = false;
                img.visible = true;

                var picNameTmp=picPath.split("/")
                tabBar.itemAt(tabBar.currentIndex).text = picNameTmp[picNameTmp.length-1]
            }
            onRejected: {
                picPath = "";
            }
        }
        onClicked: {
            fds.open();
        }
    }

    Image {
        id: background
        anchors.fill: parent
        fillMode: Image.Tile
        source: imgControl.Material.theme === Material.Dark ? "qrc:/image/bgPicDark.png": "qrc:/image/bgPicLight.png"
        z: -1
    }

    Image {
        id: img
        visible: false
//        anchors.fill: parent
        width: parent.width
        height: parent.height
        fillMode: Image.PreserveAspectFit
        source: picPath
    }

    // 旋转缩放图片
    PinchArea {
        anchors.fill: parent
        pinch.maximumRotation: 360
        pinch.minimumRotation: -360
        pinch.maximumScale: 4
        pinch.minimumScale: 0.5
        pinch.target: img

        PropertyAnimation {
            id: animation
            target: img
            properties: "rotation"
            duration: 100
        }

        onPinchStarted: {
            if(img.source != "")
                pinch.accepted = true;
        }
        onPinchUpdated: {
            img.rotation += pinch.rotation;
        }
        onPinchFinished: {
            if(img.rotation < 0)
                img.rotation += 360;
            if(img.rotation >=315)
                img.rotation -= 360;

            if(img.rotation < 45)
                animation.to = 0;
            else if(img.rotation >= 45 && img.rotation < 135)
                animation.to = 90;
            else if(img.rotation >= 135 && img.rotation < 225)
                animation.to = 180;
            else if(img.rotation >= 225 && img.rotation < 315)
                animation.to = 270;
            animation.running = true;
            pinch.accepted = false;
        }
    }
}
