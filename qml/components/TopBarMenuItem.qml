// MyMenuItem.qml
import QtQuick
import QtQuick.Controls

MenuItem {
    // anchors.verticalCenter: parent.verticalCenter
    // anchors.left: parent.left
    // anchors.leftMargin: 10
    // background: Rectangle {
    //     color: item.hovered ? "#333" : "transparent"
    //     radius: 6
    // }

    id: item

    contentItem: Text {
        text: item.text
        color: '#ffffff'
    }

    background: Rectangle {
        color: item.hovered ? "#333333" : "transparent"
        radius: 4
        anchors.fill: parent
        anchors.margins: 2
    }

}
