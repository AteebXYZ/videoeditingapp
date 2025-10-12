// TopBarMenu.qml
import QtQuick
import QtQuick.Controls

Menu {
    // opacity: 0
    // scale: 0.8

    id: root

    property string titleText: ""
    property color bgColor: "#222222"
    property bool animated: true

    title: titleText

    background: Rectangle {
        id: bgRect

        implicitWidth: 150
        // implicitHeight: childrenRect.height
        color: bgColor
        // radius: 10
        bottomLeftRadius: 10
        bottomRightRadius: 10
        clip: true
    }

    enter: Transition {
        ParallelAnimation {
            // NumberAnimation {
            //     property: "width"
            //     from: 0
            //     to: root.implicitWidth
            //     duration: 200
            //     easing.type: Easing.OutBack
            // }
            // NumberAnimation {
            //     property: "opacity"
            //     from: 0
            //     to: 1
            //     duration: 200
            //     easing.type: Easing.OutBack
            //     easing.overshoot: 1.2
            // }

            NumberAnimation {
                // easing.overshoot: 1.2

                property: "height"
                from: 0
                to: root.implicitHeight
                duration: 300
                easing.type: Easing.InOutBack
                easing.overshoot: 1
            }

        }

    }

    exit: Transition {
        ParallelAnimation {
            // NumberAnimation {
            //     property: "width"
            //     to: 0
            //     duration: 200
            //     easing.type: Easing.OutBack
            // }
            // NumberAnimation {
            //     property: "opacity"
            //     to: 0
            //     duration: 200
            //     easing.type: Easing.OutBack
            //     easing.overshoot: 1.2
            // }

            NumberAnimation {
                // easing.overshoot: 1.2

                property: "height"
                to: 0
                duration: 300
                easing.type: Easing.InOutBack
                easing.overshoot: 1
            }

        }

    }

}
