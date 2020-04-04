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
            color: root.Material.theme === Material.Dark ? "white": "black"
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
            if(drop.hasUrls){
                for(var i = 0; i < drop.urls.length; i++){
                    addPictrue(drop.urls[i]);
                }
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
            x: 30
            text: qsTr("添加图片")
            FileDialog {
                id:fds
                title: "选择文件"
                folder: shortcuts.desktop
                selectExisting: true
                selectFolder: false
                selectMultiple: false
                nameFilters: ["图片文件 (*.png *.jpg *.jpeg)"]
                onAccepted: {
                    addPictrue(fds.fileUrl.toString());
                }
            }
            onClicked: {
                fds.open();
            }
        }
        Button {
            id: addDir
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: addPic.right
            anchors.leftMargin: addPic.x
            text: qsTr("添加文件夹")

            onClicked: {
                imgControlView.model--;
            }
        }
        Button {
            id: compress
            anchors.verticalCenter: parent.verticalCenter
            x: parent.width-width-addPic.x
            text: qsTr("开始压缩")

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

                    busyIndicator.running = !isRuning;
                    busyText.text = "正在压缩图片\n压缩进度: " + now + " / " + total;
                }
            }
        }
    }

    function addPictrue(name){
        // 如果是规定图片
        if(qmlCompressControl.checkImage(name)){
            // 如果列表里没有则添加
            if(qmlCompressControl.getImgPathNameList().indexOf(name) === -1){
                qmlCompressControl.push(name);
                imgControlView.model = qmlCompressControl.getImgPathNameList();
            }
        }
    }
}
