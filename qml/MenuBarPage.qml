import QtQuick 2.13
import QtQuick.Controls 2.13
import Qt.labs.platform 1.0


Item {
    MenuBar {
        id: menuBarPage

        Menu {
            id: fileMenu
            title: qsTr("文件")
            // ...
            MenuItem {
                text: qsTr("Zoom In")
                shortcut: StandardKey.ZoomIn
                onTriggered: zoomIn()
            }

            MenuItem {
                text: qsTr("Zoom Out")
                shortcut: StandardKey.ZoomOut
                onTriggered: zoomOut()
            }
        }

        Menu {
            id: editMenu
            title: qsTr("&Edit")
            // ...
            MenuItem {
                text: qsTr("复制")
                shortcut: StandardKey.ZoomIn
                onTriggered: zoomIn()
            }

            MenuItem {
                text: qsTr("粘贴")
                shortcut: StandardKey.ZoomOut
                onTriggered: zoomOut()
            }
        }

        Menu {
            id: viewMenu
            title: qsTr("&View")
            // ...
            MenuItem {
                text: qsTr("Zoom In")
                shortcut: StandardKey.ZoomIn
                onTriggered: zoomIn()
            }

            MenuItem {
                text: qsTr("Zoom Out")
                shortcut: StandardKey.ZoomOut
                onTriggered: zoomOut()
            }
        }

        Menu {
            id: helpMenu
            title: qsTr("&Help")
            // ...
            MenuItem {
                text: qsTr("Zoom In")
                shortcut: StandardKey.ZoomIn
                onTriggered: zoomIn()
            }

            MenuItem {
                text: qsTr("Zoom Out")
                shortcut: StandardKey.ZoomOut
                onTriggered: zoomOut()
            }
        }
    }
}
