// import QtApp 1.0
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import "components"

ApplicationWindow {
    property bool menuAnimationsEnabled: true

    visible: true
    width: 800
    height: 600
    title: "Qt6 App with Soft Shadow Menus"

    SplitView {
        id: splitView

        orientation: Qt.Vertical
        anchors.fill: parent
        implicitHeight: 600

        SplitView {
            SplitView.minimumHeight: 100
            orientation: Qt.Horizontal

            MediaPool {
                // anchors.fill: parent
                // anchors.margins: 10

            }

            Viewer {
                // anchors.fill: parent
                // anchors.margins: 10

            }

            handle: Handle {
            }

        }

        Timeline {
            // anchors.fill: parent
            // anchors.margins: 10

        }

        handle: Handle {
        }

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
