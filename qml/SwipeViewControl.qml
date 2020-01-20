import QtQuick 2.13
import QtQuick.Controls 2.13
import "qrc:/controls"

SwipeView {
    anchors.fill: parent
    currentIndex: tabBar.currentIndex

    Component.onCompleted: contentItem.interactive = false

    Page1Form {
        id: page1
    }

    Page2Form {
        id: page2
    }
}
