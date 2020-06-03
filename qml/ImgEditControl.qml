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
        ToolTip.visible: hovered
        ToolTip.text: qsTr("选择图片文件，以导入到程序中")
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

    // 缩略图
    Rectangle{
        width: img.width >= img.height ? 88 : 88*img.width/img.height
        height: img.height > img.width ? 88 : 88*img.height/img.width
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        border.color: imgControl.Material.theme === Material.Dark ? "white" : "black"
        color: "transparent"
        border.width: 2
        z: 2
        Image {
            id: imgScale
            source: img.source
            width: parent.width - parent.border.width*2
            height: parent.height - parent.border.height*2
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            mirror: false
        }
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
        id: pinchArea
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
            if(pinchArea.pinch.accepted === true)
                return ;

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

    // 右侧工具栏
    property bool bMenuShown: false
    Rectangle {
        id: menuTool
        width: 80
        height: parent.height
        opacity: 0.75
        color: parent.Material.theme === Material.Dark ? "#606060" : "white"
        z: 16

        Button {
            id: menuButton
            width: 40
            height: 48
            anchors.verticalCenter: parent.verticalCenter
            x: -width
            text: "<"
            onClicked: onMenu();
        }

        Rectangle {
            width: parent.width*0.8
            height: width
            y: parent.height/3-height/2
            x: parent.width*0.1
            color: "#00000000"
            Image {
                id: imgLeft
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/image/left.png"
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: imgLeft.source = "qrc:/image/leftHover.png"
                onPressed: imgLeft.source = "qrc:/image/leftPress.png"
                onReleased: RotationAnimation{
                    target: img
                    from: img.rotation
                    to: img.rotation-90 < 0 ? 270 : img.rotation-90
                    duration: 200
                    direction: RotationAnimation.Counterclockwise
                    onStarted: {
                        imgLeft.source = "qrc:/image/leftHover.png"
                    }
                }
                onExited: imgLeft.source = "qrc:/image/left.png"
            }
        }

        Rectangle {
            width: parent.width*0.8
            height: width
            y: parent.height*2/3-height/2
            x: parent.width*0.1
            color: "#00000000"
            Image {
                id: imgRight
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/image/right.png"
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: imgRight.source = "qrc:/image/rightHover.png"
                onPressed: imgRight.source = "qrc:/image/rightPress.png"
                onReleased: RotationAnimation{
                    target: img
                    from: img.rotation
                    to: img.rotation+90 > 270 ? 0 : img.rotation+90
                    duration: 200
                    direction: RotationAnimation.Clockwise
                    onStarted: {
                        imgRight.source = "qrc:/image/rightHover.png"
                    }
                }
                onExited: imgRight.source = "qrc:/image/right.png"
            }
        }

        transform: Translate {
            id: menuTranslate
            x: width
            Behavior on x {
                NumberAnimation {
                    duration: 400;
                    easing.type: Easing.OutQuad
                }
            }
        }
    }
    MouseArea {
        id: menuArea
        z: -1
        x: 0
        y: 0
        width: parent.width - menuTool.width
        height: parent.height
        enabled: bMenuShown
        onClicked: onMenu();
    }

    function onMenu(){
        menuTranslate.x = bMenuShown ? width : width-menuTool.width;
        menuButton.text = bMenuShown ? "<" : ">";
        menuArea.z = bMenuShown ? -1 : 16;
        bMenuShown = !bMenuShown;
    }

    function addPicture(path){
        picPath = path;
        imgControlButton.visible = false;
        img.visible = true;

        var picNameTmp=picPath.split("/")
        tabBar.itemAt(tabBar.currentIndex).text = picNameTmp[picNameTmp.length-1]
    }
}
