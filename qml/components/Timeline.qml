import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: timeline

    property color bgColor: "#444"

    implicitHeight: 100
    SplitView.minimumHeight: 75
    color: bgColor

    Text {
        anchors.centerIn: parent
        text: "Timeline"
        color: "#ccc"
        font.pixelSize: 20
    }

}
