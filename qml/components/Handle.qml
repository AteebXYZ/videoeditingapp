import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    implicitHeight: 4
    implicitWidth: 4
    color: SplitHandle.pressed ? '#d6d6d6' : (SplitHandle.hovered ? Qt.lighter('#6a6a6a', 1.1) : '#00ffffff')
}
