import QtQuick 2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0

Page {
    id: page
    title: qsTr("Edit init field")

    header: ToolBar {
        contentHeight: label.implicitHeight + 4

        RowLayout {
            Button {
                id: button

                text: qsTr("Start")
                onClicked: {
                    fieldModel.endEdit()
                    stackView.pop()
                }
            }
            Item {
                width: 20
            }
            Label {
                id: label
                font.pixelSize: Qt.application.font.pixelSize * 0.9
                text: qsTr("Left button click -\n add fly with selected T\nRight button click -\n remove last fly")
            }
            Item {
                width: 20
            }
            Label {
                text: qsTr("Select current T:")
            }
            TextField {
                enabled: false
                text: slider.value
            }
            Slider {
                id: slider
                from: 1
                to: 20
                stepSize: 1
                value: 10
            }
        }
    }

    FieldEdit {
        anchors.fill: parent
    }


    Component.onCompleted: {
        fieldModel.startEdit()

    }
}
