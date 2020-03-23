import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.12

Page {
    Row {
        anchors.fill: parent;
        TabBar {
            id: tabBar
            currentIndex: 0
            width: parent.width - addButton.width

            onCurrentIndexChanged: {
                stackLayout.currentIndex = tabBar.currentIndex
            }

            Component.onCompleted: {
                tabBar.addItem(tabButton.createObject(tabBar))
                this.currentIndex = 0
            }

            Component {
                id: tabButton
                TabButton {
                    text: qsTr("标签")

                    Button {
                        anchors.right: parent.right
                        text: "✕"
                        width: height
                        flat: true

                        onClicked: {
                            // 获取点击x的按钮，判断方法为按钮X的parent == 遍历的tabBar.itemAt(i)
                            var index = -1
                            for(var i=0;i<tabBar.count;i++){
                                if(tabBar.itemAt(i) === this.parent){
                                    index = i;
                                }
                            }

                            if(tabBar.count>1){
                                stackLayout.children[index].destroy();

                                tabBar.removeItem(index);
                                tabBar.currentIndex = tabBar.count-1
                            }
                            else{
                                tabBar.removeItem(0);
                                tabBar.addItem(tabButton.createObject(tabBar))
                                tabBar.currentIndex = tabBar.count - 1
                                stackLayout.children[0].destroy();
                                stackLayout.creatItem();
                            }
                        }
                    }
                }
            }
        }

        Button {
            id: addButton
            text: "+"
            width: height
            flat: true
            onClicked: {
                if(tabBar.itemAt(tabBar.count-1).text !=="标签"){
                    tabBar.addItem(tabButton.createObject(tabBar))
                    tabBar.currentIndex = tabBar.count - 1

                    stackLayout.creatItem();
                    stackLayout.currentIndex = stackLayout.count - 1
                }
            }
        }
    }

    StackLayout {
        id: stackLayout
        width: parent.width
        height: parent.height - tabBar.height
        y: tabBar.height
        currentIndex: tabBar.currentIndex

        function creatItem(){
            var item = Qt.createComponent("qrc:/controls/ImgEditControl.qml")
            var sprite = item.createObject(stackLayout)
        }

        Component.onCompleted: {
            creatItem();
        }
    }
}
