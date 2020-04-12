import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.12

Item{
    id: imgControl

    width: parent.width
    height: parent.height

    property string picPath: ""

    clip: true

    Button {
        id: imgControlButton
        //        flat: true
        anchors.centerIn: parent
        text: qsTr("选择或拖入图片")
        z: 50

        FileDialog {
            id:fds
            title: "选择文件"
            folder: shortcuts.desktop
            selectExisting: true
            selectFolder: false
            selectMultiple: false
            nameFilters: ["图片文件 (*.png *.jpg *.jpeg *.bmp)"]
            onAccepted: {
                addPicture(fds.fileUrl);
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
        x: imgControl.width/2-width/2
        y: imgControl.height/2-height/2
        fillMode: Image.PreserveAspectFit
        mirror: false
        source: picPath
        z: 1
    }

    // 快速拖入图片
    DropArea {
        anchors.fill: parent
        Rectangle {
            anchors.fill: parent
            color: "gray"
            opacity: 0.4

            visible: parent.containsDrag

            Text {
                anchors.centerIn: parent

                opacity: 1
                text: "+"
                font.bold: true
                font.pixelSize: 166
                color: parent.Material.theme === Material.Dark ? "white":"black"
            }
        }
        onDropped: {
            if(drop.hasUrls && img.source == ""){
                addPicture(drop.urls[0]);
            }
        }
    }

    // 旋转缩放图片
    PinchArea {
        anchors.fill: parent
        pinch.maximumRotation: 360
        pinch.minimumRotation: -360
        pinch.maximumScale: 5
        pinch.minimumScale: 0.1
        pinch.target: img
        pinch.dragAxis:Pinch.XAndYAxis

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

    // 拖拽图片
    MouseArea {
        anchors.fill: img
        drag.target: img
        z: 4
//        drag.axis: Drag.XAndYAxis
        //这里使图片不管是比显示框大还是比显示框小都不会被拖拽出显示区域
//        drag.minimumX: (img.width > imgControl.width) ? (imgControl.width - img.width) : 0
//        drag.minimumY: (img.height > imgControl.height) ? (imgControl.height - img.height) : 0
//        drag.maximumX: (img.width > imgControl.width) ? 0 : (imgControl.width - img.width)
//        drag.maximumY: (img.height > imgControl.height) ? 0 : (imgControl.height - img.height)

        onDoubleClicked: {
            console.log(img.width+" "+img.height+"   "+imgControl.width+" "+imgControl.height)
            console.log(img.x+" "+img.y+"   "+imgControl.x+" "+imgControl.y)
            console.log(img.scale)
        }

        //使用鼠标滚轮缩放
        onWheel: {
            //每次滚动都是120的倍数
            var zoom = 0.96
            var datla = wheel.angleDelta.y/120;
            if(datla > 0)
            {
                if(img.scale/zoom<=5)
                    img.scale = img.scale/zoom
                else
                    img.scale = 5
            }
            else
            {
                if(img.scale*zoom>=0.1)
                    img.scale = img.scale*zoom
                else
                    img.scale = 0.1
            }
        }
    }

    function addPicture(path){
        picPath = path;
        imgControlButton.visible = false;
        img.visible = true;

        var picNameTmp=picPath.split("/")
        tabBar.itemAt(tabBar.currentIndex).text = picNameTmp[picNameTmp.length-1]
    }
}
