import QtQuick 2.0
import QtGraphicalEffects 1.12

Image {
    property bool dead: false
    property int t: 0

    id: image
    source: "qrc:/fly.png"

    Row {
        visible: t > 0
        spacing: 1
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        Repeater {
            model: t
            delegate: Rectangle {
                color: "blue"
                width: image.width / 20 - 1
                height: width * 5
            }
        }
    }

    Colorize {
        anchors.fill: parent
        source: parent
        hue: 0.0
        saturation: 0.0
        visible: dead
    }

    Rectangle {
        visible: dead
        color: "transparent"
        border.color: "red"
        border.width: 3
        radius: width * 0.3
        anchors.fill: parent
        z: 1
    }
}
