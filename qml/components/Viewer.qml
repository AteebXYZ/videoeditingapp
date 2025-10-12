import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    // property color borderColor: "#111111"
    // property int borderWidth: 2

    id: viewer

    property color bgColor: "#222222"

    color: bgColor

    Text {
        anchors.centerIn: parent
        text: "Viewer"
        color: "#ccc"
        font.pixelSize: 20
        clip: false
    }

}
