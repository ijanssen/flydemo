import QtQuick 2
import QtQuick.Controls 2.5

import org.flydemo 1.0


ApplicationWindow {
    id: window
    width: 800
    height: 900
    visible: true
    title: qsTr("Fly simulation")

    property int sizen: 0

    function startWithRandomInit(num)
    {
        stackView.pop()
        fieldModel.initRandom(num)
    }

    function openFieldEditor()
    {
        stackView.pop()
        stackView.push("FieldEditPage.qml")
    }

    FieldModel {
        id: fieldModel
    }

    header: ToolBar {
        contentHeight: hlabel.implicitHeight

        Label {
            id: hlabel
            text: stackView.currentItem.title// + " " + stackView.depth
            anchors.centerIn: parent
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: FieldPage {
            id: field

        }
    }

    Component.onCompleted: {
        stackView.push("InitPage.qml");
    }
}
