import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.12

Item {
    property alias img: img
    property alias fileName: fileName

    id: root

    Image {
        id: img
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width*0.5
        height: parent.height - fileName.height
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: fileName
        height: 20
        width: parent.width*0.9
        anchors.top: img.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter

        font.pointSize: 15
        elide: Text.ElideMiddle


        color: root.Material.theme === Material.Dark ? "white": "gray"
    }
}
