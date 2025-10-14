import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs

Rectangle {
    id: mediaPool

    SplitView.minimumWidth: 100
    SplitView.minimumHeight: 100
    implicitWidth: 500
    color: "#333"
    Component.onCompleted: {
        MediaPoolManager.load();
    }

    GridView {
        id: gridView

        // interactive: false
        anchors.fill: parent
        anchors.margins: 10
        cellWidth: 150
        cellHeight: 100
        // clip: true
        model: MediaPoolManager.clips

        delegate: Rectangle {
            width: gridView.cellWidth - 4 // leave 2px margin each side
            height: gridView.cellHeight - 4
            color: "#555"
            radius: 4

            Column {
                anchors.fill: parent
                anchors.margins: 2
                spacing: 2

                Image {
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: modelData["thumbnail"]
                    width: parent.width
                    height: parent.height - 20
                    fillMode: Image.PreserveAspectCrop
                }

                Text {
                    text: modelData["name"]
                    color: "white"
                    font.pixelSize: 12
                    elide: Text.ElideRight
                    horizontalAlignment: Text.AlignHCenter
                    width: parent.width
                }

            }

        }

    }

    FileDialog {
        id: fileDialog

        title: "Import clip"
        nameFilters: ["Video Files (*.mp4 *.mov *.avi *.mkv)", "All files (*)"]
        fileMode: FileDialog.OpenFiles
        options: FileDialog.DontUseNativeDialog
        onAccepted: {
            let fileUrl = fileDialog.selectedFile.toString();
            let fileName = fileUrl.split("/").pop();
            MediaPoolManager.addClip(fileName, fileUrl);
            MediaPoolManager.save();
        }
    }

    Menu {
        id: contextMenu

        MenuItem {
            text: "Import clip"
            onTriggered: fileDialog.open()
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

    Connections {
        function onClipsChanged() {
            gridView.model = MediaPoolManager.clips;
        }

        target: MediaPoolManager
    }

}
