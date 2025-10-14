// import QtApp 1.0
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import "components"

ApplicationWindow {
    // SplitView {
    //     id: splitView
    //     orientation: Qt.Vertical
    //     anchors.fill: parent
    //     implicitHeight: 0
    //     SplitView {
    //         SplitView.minimumHeight: 100
    //         SplitView.minimumWidth: 100
    //         orientation: Qt.Horizontal
    //         MediaPool {
    //             // anchors.fill: parent
    //             // anchors.margins: 10
    //             id: mediaPool
    //         }
    //         Viewer {
    //             // anchors.fill: parent
    //             // anchors.margins: 10
    //             id: viewer
    //             viewerSource: mediaPool.viewerSource
    //         }
    //         handle: Handle {
    //         }
    //     }
    //     Timeline {
    //         // anchors.fill: parent
    //         // anchors.margins: 10
    //     }
    //     handle: Handle {
    //     }
    // }

    id: appWindow

    property bool menuAnimationsEnabled: true

    visible: true
    width: 1000
    height: 700
    title: "Advanced QML Tiling Window Manager"

    DockArea {
    }

    menuBar: MenuBar {
        TopBarMenu {
            animated: menuAnimationsEnabled
            titleText: "File"

            TopBarMenuItem {
                text: "New"
            }

            TopBarMenuItem {
                text: "Open"
            }

            TopBarMenuItem {
                text: "Save"
            }

            TopBarMenuItem {
                text: "Save As"
            }

            MenuSeparator {

                contentItem: Rectangle {
                    implicitHeight: 1
                    color: '#7f7f7f'
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.margins: 4
                }

            }

            TopBarMenuItem {
                text: "Exit"
                onTriggered: Qt.quit()
            }

        }

        TopBarMenu {
            animated: menuAnimationsEnabled
            titleText: "Edit"

            TopBarMenuItem {
                text: "Copy"
            }

            TopBarMenuItem {
                text: "Paste"
            }

        }

    }

}
