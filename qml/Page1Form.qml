import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Material 2.12
import QmlCompressControl 1.0
import "qrc:/controls"

Page {
    width: 600
    height: 400

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        implicitWidth: parent.width
        implicitHeight: implicitWidth
        opacity: running ? 0.0 : 1.0
        running: true
        z: 10

        Text {
            id: busyText
            anchors.centerIn: parent
            font.pointSize: 15
            color: root.Material.theme === Material.Dark ? "white": "#303030"
        }
        contentItem: MyBusyIndicator{}
    }

    GridView {
        id: imgControlView

        anchors.fill: parent
        cellHeight: 180
        cellWidth: 160

        // model和view，显示已导入的图片
        model: qmlCompressControl.getImgPathNameList()
        delegate: ImgComoressControl{
            id: imgControl
            width: 160
            height: 160

            Component.onCompleted: {
                img.source = qmlCompressControl.getImgPathName(index);
                fileName.text = "<b>"+qmlCompressControl.getImgName(index)+"</b>";
            }
        }

        // 内部存有导入的图片的路径等信息，并提供压缩图片等接口
        QmlCompressControl {
            id: qmlCompressControl
        }
    }
    Text {
        id: noticeText
        anchors.centerIn: parent
        color: parent.Material.theme === Material.Dark ? "white":"#303030"
        font.pixelSize: 16
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("可以拖拽图片到程序中\n也可以拖拽文件夹到程序中")
    }

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
                color: parent.Material.theme === Material.Dark ? "white":"#303030"
            }
        }
        onDropped: {
            if(drop.hasUrls){
                for(var name of drop.urls){
                    if(qmlCompressControl.isDirectory(name)){
                        var imgs = qmlCompressControl.getImgInDirectory(name);
                        for(var img of imgs){
                            addPictrue(img.toString());
                        }
                    }
                    else
                        addPictrue(name);
                }
                imgControlView.model = qmlCompressControl.getImgPathNameList();
            }
        }
    }

    footer: Item {
        id: footRow
        height: 50
        width: parent.width

        Button {
            id: addPic
            anchors.verticalCenter: parent.verticalCenter
            x: 15
            text: qsTr("添加图片")
            ToolTip.visible: hovered
            ToolTip.text: qsTr("选择图片文件，以导入到程序中")
            FileDialog {
                id: fd
                title: "选择文件"
                folder: shortcuts.desktop
                selectExisting: true
                selectFolder: false
                selectMultiple: true
                nameFilters: ["图片文件 (*.png *.jpg *.jpeg)"]
                onAccepted: {
                    for(var name of fd.fileUrls){
                        addPictrue(name.toString());
                    }
                    imgControlView.model = qmlCompressControl.getImgPathNameList();
                }
            }
            onClicked: {
                fd.open();
            }
        }
        Button {
            id: addDir
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: addPic.right
            anchors.leftMargin: addPic.x
            text: qsTr("添加文件夹")
            ToolTip.visible: hovered
            ToolTip.text: qsTr("选择文件夹，以导入文件夹中所有的图片")

            FileDialog {
                id: fds
                title: "选择文件"
                folder: shortcuts.desktop
                selectExisting: true
                selectFolder: true
                selectMultiple: true
                onAccepted: {
                    for(var dir of fds.fileUrls){
                        var imgs = qmlCompressControl.getImgInDirectory(dir);
                        for(var img of imgs){
                            addPictrue(img.toString());
                        }
                    }
                    imgControlView.model = qmlCompressControl.getImgPathNameList();
                }
            }
            onClicked: {
                fds.open();
            }
        }
        Button {
            id: clearPic
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: addDir.right
            anchors.leftMargin: addPic.x
            text: qsTr("清除图片")
            ToolTip.visible: hovered
            ToolTip.text: qsTr("清除已经导入的图片")
            onClicked: {
                qmlCompressControl.clear();
                imgControlView.model = qmlCompressControl.getImgPathNameList();
                noticeText.opacity = 1;
                compress.toolTipText = "请先导入图片";
            }
        }
        Text {
            id: textJpg1
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: clearPic.right
            anchors.leftMargin: addPic.x
            color: parent.Material.theme === Material.Dark ? "white":"#303030"
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("JPEG压缩质量")
        }
        Slider {
            id: slider
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: textJpg1.right
            anchors.leftMargin: 10
            width: 140
            height: 20
            from: 1
            to: 100
            value: 60
            stepSize: 1
            onValueChanged: {
                textJpg2.text = slider.value.toString();
                var num = 101 - slider.value;
                qmlCompressControl.setQuality(num);
            }
        }
        Text {
            id: textJpg2
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: slider.right
            anchors.leftMargin: 10
            color: parent.Material.theme === Material.Dark ? "white":"#303030"
            height: parent.height
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("60")
        }
        Button {
            id: change
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: compress.left
            anchors.rightMargin: addPic.x
            text: qsTr("改变主题")
            ToolTip.visible: hovered
            ToolTip.text: qsTr("在深色主题、浅色主题中切换")
            onClicked: {
                changeTheme();
            }
        }
        Button {
            property string toolTipText: "请先导入图片"

            id: compress
            anchors.verticalCenter: parent.verticalCenter
            x: parent.width-width-addPic.x
            text: qsTr("开始压缩")
            ToolTip.visible: hovered
            ToolTip.text: toolTipText

            onClicked: {
                if(qmlCompressControl.getImgPathNameList().length > 0){
                    qmlCompressControl.compress();
                }
            }
            Connections {
                target: qmlCompressControl;
                onReturnIsRuning:{
                    compress.enabled = !isRuning;
                    addDir.enabled = !isRuning;
                    addPic.enabled = !isRuning;
                    slider.enabled = !isRuning;
                    clearPic.enabled = !isRuning;

                    busyIndicator.running = !isRuning;
                    busyText.text = "正在压缩图片\n压缩进度: " + now + " / " + total;
                    if(isRuning === false){
                        successPopup.open();
                        popupText.text = "所有的图片文件已经压缩完成\n"+
                                "已经保存到图片所在的文件夹中\n"+
                                "并命名为\"图片名+_Compress\"\n\n"+
                                "总耗时:"+time+"\n"+
                                "压缩前:"+orignalSize+"  压缩后:"+resultSize+"\n"+
                                " 压缩率:"+ratio;
                    }
                }
            }
        }
    }
    // 模糊遮罩
    Rectangle{
        id: blurRect
        x: footRow.x
        y: footRow.y
        width: footRow.width
        height: footRow.height
        color: parent.Material.theme !== Material.Dark ? "white":"#303030"
        opacity: 0.6
    }
    // 压缩成功提示框
    Popup {
        id: successPopup
        width: 360
        height: 250
        x: (root.width-width)/2
        y: (root.height-height)/2
        modal: true
        Text {
            id: popupText
            anchors.horizontalCenter: parent.horizontalCenter
            y: (parent.height-height-okButton.height)/2
            color: parent.Material.theme === Material.Dark ? "white":"#303030"
            text: qsTr("所有的图片文件已经压缩完成\n已经保存到图片所在的文件夹中\n并命名为\"图片名+_Compress\"")
            font.pixelSize: 15
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Button {
            id: okButton
            anchors.bottom: parent.bottom
            width: parent.width
            text: qsTr("好的")
            onClicked: {
                successPopup.close();
            }
        }
    }

    function addPictrue(pathName){
        // 如果是规定图片
        if(qmlCompressControl.checkImage(pathName)){
            // 如果列表里没有则添加
            if(qmlCompressControl.getImgPathNameList().indexOf(pathName) === -1){
                compress.toolTipText = "点击按钮，开始压缩图片";
                qmlCompressControl.push(pathName);
                noticeText.opacity = 0;
            }
        }
    }
}
