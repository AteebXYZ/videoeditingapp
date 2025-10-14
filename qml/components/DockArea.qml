// ...existing code...
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

SplitView {
    id: splitView

    orientation: Qt.Vertical
    anchors.fill: parent
    implicitHeight: 0

    SplitView {
        SplitView.minimumHeight: 100
        SplitView.minimumWidth: 100
        orientation: Qt.Horizontal

        MediaPool {
            // anchors.fill: parent
            // anchors.margins: 10
            id: mediaPool
        }

        Viewer {
            // viewerSource: mediaPool.viewerSource

            // anchors.fill: parent
            // anchors.margins: 10
            id: viewer
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
