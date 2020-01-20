import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.12

ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 600
    title: qsTr("抚琴曲委婉")

    function changeTheme(){
        if(root.Material.theme === Material.Dark ){
            root.Material.theme = Material.Light;
        }else{
            root.Material.theme = Material.Dark;
        }
    }

    SwipeViewControl {
        id: swipeView
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("压缩图片")
        }
        TabButton {
            text: qsTr("编辑图片")
        }
    }

}
