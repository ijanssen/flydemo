import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0

Page {
    id: page
    title: qsTr("Simulation")

    header: ToolBar {
        contentHeight: toolButton1.implicitHeight

        RowLayout {
            spacing: 10
            ToolButton {
                id: toolButton1
                enabled: !fieldModel.running
                text: "\u25B6"
                font.pixelSize: Qt.application.font.pixelSize * 2.0
                onClicked: {
                    fieldModel.start()
                }
            }

            ToolButton {
                id: toolButton2
                enabled: fieldModel.running
                text: "\u25A0"
                font.pixelSize: Qt.application.font.pixelSize * 2.0
                onClicked: {
                    fieldModel.stop()
                }
            }

            Label {
                width: 100
            }
            Label {
                text: qsTr("Time: ") + fieldModel.currTime / 1000 + qsTr(" sec")
            }
        }
    }


    Field {
        anchors.fill: parent
    }
}
