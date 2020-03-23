import QtQuick 2.13
import QtGraphicalEffects 1.0

Item {
    id: root
    property var zoom: 0.35

    Rectangle {
        width: parent.width
        height: width
        color:Qt.rgba(0.1,0.1,0.1,0.4)
    }

    Rectangle {
        id: rect
        width: parent.width
        height: width
        color: Qt.rgba(0, 0, 0, 0)
        radius: width / 2
        border.width: width / 10
        visible: false
    }

    ConicalGradient {
        width: rect.width * zoom
        height: width
        anchors.centerIn: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#9932cc" }
            GradientStop { position: 1.0; color: "#00ffff" }
        }
        source: rect

        Rectangle {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: rect.border.width * zoom
            height: width
            radius: width / 2
            color: "#9932cc"
        }

        RotationAnimation on rotation {
            from: 0
            to: 360
            duration: 1000
            loops: Animation.Infinite
        }
    }
}
