import QtQuick 2.0

Rectangle {
    id: msg


    function showMessage(x, y, message, yf = false) {
        banner.text = message
        playMessage.start()
        msg.x = x
        msg.y = yf ? y - msg.height : y

    }


    color: "lightblue"
    border.width: 1
    width: banner.width * 1.3
    height: banner.height * 1.3
    radius: 5
    opacity: 0
    Text {
        id: banner
        anchors.centerIn: parent
    }
    SequentialAnimation {
        id: playMessage
        running: false
        NumberAnimation { target: message;
            property: "opacity";
            to: 1.0;
            duration: 200
            easing.type: Easing.InOutQuad
        }
        PauseAnimation  { duration: 5000 }
        NumberAnimation { target: message;
            property: "opacity";
            to: 0.0;
            duration: 2000}
    }
}

