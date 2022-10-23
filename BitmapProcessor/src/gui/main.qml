import QtQuick 2.15
import QtQuick.Window 2.15

import AppEngine.Models 1.0
import AppEngine 1.0

import "misc"
import "delegates"

Window {
    id: _root
    width: 700
    height: 900
    visible: true
    title: qsTr("Bitmap processor")
//-----------------------------------------------------------------------------------
    Rectangle {
        id: _filterRect
        height: 36
        radius: 8
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            margins: 3
        }
        border {
            color: "black"
            width: 2
        }

        TextEdit {
            id: _filterInput
            height: parent.height
            anchors {
                fill: parent
                margins: 5
            }

            verticalAlignment: Text.AlignVCenter
            font {
                bold: true
                capitalization: Font.AllUppercase
                pointSize: 14
            }
            Text {
                anchors {
                    fill: parent
                }
                verticalAlignment: Text.AlignVCenter
                text: "Enter file suffix..."
                color: "gray"
                visible: !_filterInput.text
                font {
                    bold: true
                    capitalization: Font.AllUppercase
                    pointSize: 14
                }
            }
            onTextChanged: FileListFilterModel.setSuffix(text)
        }
    }
//-----------------------------------------------------------------------------------
    Rectangle {
        id: _listRect
        height: 36
        radius: 8
        anchors {
            top: _filterRect.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 3
        }
        border {
            color: "black"
            width: 2
        }

        ListView {
            id: _fileView
            anchors {
                fill: parent
                margins: 5
            }
            spacing: 3
            clip: true
            model: FileListFilterModel
            delegate: FileListDelegate {
                width: ListView.view.width
                fileName: model.fileName
                fileIcon: model.fileIcon
                fileSize: "[" + model.fileSize + "]"
                onClicked: AppEngine.processImage(model.fileData)
            }
        }
    }
//-----------------------------------------------------------------------------------
    Loader {
        id: _loader
        anchors.centerIn: parent
    }
//-----------------------------------------------------------------------------------
    Component {
        id: _popup
        Popup {
            width: _root.width * 0.8
        }
    }
//-----------------------------------------------------------------------------------
    Connections {
        target: AppEngine
        function onWrongFormat(reason) {
            _loader.sourceComponent = _popup
            _loader.item.text = reason
        }
    }
//-----------------------------------------------------------------------------------
    Connections {
        target: _loader.item
        function onOkClicked() {
            _loader.sourceComponent = undefined
        }
    }
}
