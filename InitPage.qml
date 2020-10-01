import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0

Page {
    id: page
    title: qsTr("Init")

    ColumnLayout{
        spacing: 2
        anchors.centerIn: parent

        Row {
            spacing: 20
            Label {
                text: qsTr("Size M: ")
            }

            TextField {
                enabled: false
                text: sliderm.value
            }
        }

        Slider {
            id: sliderm
            from: 2
            to: 20
            value: 10
            stepSize: 1
        }


        Row {
            spacing: 20
            Label {
                text: qsTr("Size N: ")
            }


            TextField {
                enabled: false
                text: slidern.value
            }
        }

        Slider {
            id: slidern
            from: 1
            to: 49
            value: 10
            stepSize: 1
        }

        Row {
            spacing: 20
            Label {
                text: qsTr("Random generation: ")
            }

            Switch {
                id: genon
            }

            TextField {
                visible: genon.checked
                enabled: false
                text: slidernum.value
            }
        }

        Slider {
            id: slidernum
            visible: genon.checked
            from: 1
            to: 500
            value: 100
            stepSize: 1
        }


        Button {
            id: button

            text: qsTr("Start")
            onClicked: {
                sizen = slidern.value
                fieldModel.init(sliderm.value, slidern.value)
                if (genon.checked)
                    window.startWithRandomInit(slidernum.value)
                else
                    window.openFieldEditor()
            }
        }
    }
}
