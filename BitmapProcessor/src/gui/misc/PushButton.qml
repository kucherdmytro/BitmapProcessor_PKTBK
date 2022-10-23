import QtQuick 2.15

Rectangle {
    id: _root

    property alias text: _buttonText.text

    signal clicked()

    color: "transparent"

    implicitHeight: 60
    implicitWidth: 200

    Rectangle {
        id: _background
        color: "white"
        radius: 10
        border.color: "black"
        border.width: 3

        anchors.fill: parent
        anchors.margins: 3

        MouseArea {
            id: _mouseArea
            anchors.fill: parent
            onPressed: {
                _root.scale = 0.98
                _background.color = "lightgrey"
            }
            onReleased: {
                _root.scale = 1.0
                _background.color = "white"
            }
            onClicked: _root.clicked()
        }

        Text {
            id: _buttonText
            text: qsTr("text")

            anchors.fill: parent
            anchors.margins: 5
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 14
        }
    }
}
