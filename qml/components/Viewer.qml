import QtMultimedia
import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    // property color borderColor: "#111111"
    // property int borderWidth: 2

    id: viewer

    property color bgColor: "#222222"
    property url viewerSource: ""

    SplitView.minimumWidth: 100
    SplitView.minimumHeight: 100
    color: bgColor

    Item {
        anchors.fill: parent

        VideoOutput {
            id: videoOutput

            anchors.fill: parent
        }

        MediaPlayer {
            id: mediaPlayer

            autoPlay: true
            source: viewerSource
            loops: MediaPlayer.Infinite
            videoOutput: videoOutput

            audioOutput: AudioOutput {
            }

        }

    }

}
