import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Dialogs 1.2
import MusicReader 1.0
import MusicPlayer 1.0

Window {
    id: root

    Material.theme: Material.Light
    Material.accent: Material.DeepOrange

    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    FileDialog {
        id: fileDialog
        title: "Please choose a sheet-music image"
        folder: shortcuts.home
        selectMultiple: false
        onAccepted: {
            imageView.source = fileDialog.fileUrl
        }
    }

    Button {
        id: openFileButton

        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        text: "Open File"
        onPressed: fileDialog.open()
    }

    Image {
        id: imageView
        anchors.left: parent.left
        anchors.top: openFileButton.bottom
        anchors.right: parent.right
        anchors.bottom: readSongButton.top

        fillMode: Image.PreserveAspectFit
    }

    Button {
        id: readSongButton

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        height: 50
        anchors.margins: 5

        text: "Read Song"
        onPressed: MusicReader.readSong(imageView.source)
    }

    Button {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter
        height: 50
        anchors.margins: 5

        text: "Play Music"
        onPressed: MusicPlayer.playSong()
    }
}
