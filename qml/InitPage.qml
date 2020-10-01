import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0

Page {
    id: page
    title: qsTr("Init")

    property int ssize: 10

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

        Item {
            height: 30
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

        Rectangle {
            id: cell
            width: 150
            height: 150

            border.width: 1


            Component.onCompleted: {
                var n = Math.ceil(Math.sqrt(10));
                ssize = (cell.width - 2) / n - 2;

            }
            Connections {
                target: slidern
                function onValueChanged() {
                    var n = Math.ceil(Math.sqrt(slidern.value));
                    ssize = (cell.width - 2) / n - 2;
                }
            }


            Flow {
                anchors.fill: parent
                anchors.margins: 1
                spacing: 2
                Repeater {
                    model: slidern.value
                    delegate: Fly {
                        dead: false
                        width: ssize
                        height: ssize
                    }
                }
            }
        }

        Item {
            height: 30
        }

        Row {
            spacing: 20
            Label {
                text: qsTr("Random generation: ")
            }

            Switch {
                id: genon
            }

        }

        Row {
            visible: genon.checked

            spacing: 20
            Label {
                text: qsTr("Ammount: ")
            }

            TextField {
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

        Item {
            height: 30
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
