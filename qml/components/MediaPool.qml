import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs // <-- for FileDialog

Rectangle {
    id: mediaPool

    property color bgColor: "#333"
    property url viewerSource: ""

    implicitWidth: 200
    implicitHeight: 150
    SplitView.minimumWidth: 100
    SplitView.minimumHeight: 100
    color: bgColor

    GridView {
        anchors.fill: parent
        anchors.margins: 10
        cellWidth: 110
        cellHeight: 70
        // spacing: 10
        model: 10
        clip: true

        delegate: Rectangle {
            width: 100
            height: 60
            color: "#555"

            Text {
                anchors.centerIn: parent
                color: "white"
                text: "Clip " + (index + 1)
            }

        }

    }

    // --- FileDialog for importing clips ---
    FileDialog {
        // TODO: process the file here (load, copy, etc.)

        id: fileDialog

        title: "Import clip"
        nameFilters: ["Video Files (*.mp4 *.mov *.avi)", "All files (*)"]
        onAccepted: {
            console.log("Selected file:", fileDialog.selectedFile.toString());
            mediaPool.viewerSource = fileDialog.selectedFile.toString();
        }
        onRejected: {
            console.log("File import canceled");
        }
    }

    Menu {
        id: contextMenu

        MenuItem {
            text: "Import clip"
            onTriggered: fileDialog.open() // <-- open native FileDialog
        }

    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: (mouse) => {
            if (mouse.button === Qt.RightButton)
                contextMenu.popup();

        }
    }

}
