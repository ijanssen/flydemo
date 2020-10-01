import QtQuick 2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0

Page {
    id: page
    title: qsTr("Edit init field")

    header: ToolBar {
        contentHeight: button.implicitHeight

        RowLayout {
            Label {
                text: qsTr("Left button click - add fly with selected T, Right button click - remove last fly")
            }
            Slider {
                id: slider
                from: 1
                to: 20
                value: 10
            }
            Button {
                id: button

                text: qsTr("Start")
                onClicked: {
                    fieldModel.endEdit()
                    stackView.pop()
                }
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
