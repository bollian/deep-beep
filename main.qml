import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import MusicReader 1.0
import MusicPlayer 1.0

Window {
    id: root

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")


    Button {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        height: 50

        text: "Play Music"
        onPressed: MusicPlayer.playSong()
    }
}
