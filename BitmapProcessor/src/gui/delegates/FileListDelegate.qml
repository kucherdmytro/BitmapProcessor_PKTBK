import QtQuick 2.15

Rectangle {
    id: _root

    property alias fileName: _fileName.text
    property alias fileIcon: _fileIcon.source
    property alias fileSize: _fileSize.text
    signal clicked()

    implicitWidth: 500
    implicitHeight: 64
    color: "transparent"
    radius: 8
    border.width: 1

    Image {
        id: _fileIcon
        width: height
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            margins: 3
        }
        antialiasing: true
        fillMode: Image.PreserveAspectFit
        source: "qrc:/img/barch.png"
    }

    Text {
        id: _fileName
        text: qsTr("Text")
        anchors.right: _fileSize.left
        anchors {
            left: _fileIcon.right
            top: parent.top
            bottom: parent.bottom
            margins: 3
        }
        font.pointSize: 16
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
    }

    Text {
        id: _fileSize
        text: qsTr("[size]")
        width: contentWidth
        anchors {
            right: parent.right
            top: parent.top
            bottom: parent.bottom
            margins: 3
        }
        font.pointSize: 16
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        id: _processArea
        anchors.leftMargin: 3
        anchors {
            right: parent.right
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            margins: 3
        }

        onClicked: _root.clicked()
    }
}
