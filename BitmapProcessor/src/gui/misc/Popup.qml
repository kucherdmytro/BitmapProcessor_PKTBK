import QtQuick 2.15
import QtQuick.Window 2.15

Rectangle {
    id: _root

    property alias text: _messageText.text
    signal okClicked()

    implicitHeight: 200
    implicitWidth: 400

    border.width: 3
    border.color: "black"
    radius: 10

    Text {
        id: _messageText
        text: qsTr("Text")
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: _button.top
            margins: 5
        }

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 14
    }

    PushButton {
        id: _button
        text: "OK"

        height: 50

        anchors {
            bottom: parent.bottom
            right: parent.right
            margins: 5
        }
        onClicked: okClicked()
    }
}
