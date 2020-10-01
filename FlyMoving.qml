import QtQuick 2.0

Fly {
    id: fly

    function startMove(nx, ny) {
        if (nx === x && ny > y)
            rotation = 180;
        else if (nx === x && ny < y)
            rotation = 0;
        else if (nx < x && ny < y)
            rotation = 315;
        else if (nx < x && ny === y)
            rotation = 270;
        else if (nx < x && ny > y)
            rotation = 225;
        else if (nx > x && ny < y)
            rotation = 45;
        else if (nx > x && ny === y)
            rotation = 90;
        else if (nx > x && ny > y)
            rotation = 135;

        xmove.to = nx;
        ymove.to = ny;
        flyAnimation.start()
        timer.start()
    }

    Timer {
        id: timer
        interval: 500
        running: false
        repeat: false
//        onTriggered: fly.destroy()
    }

    Connections {
        target: flyAnimation
        function onFinished() {
            fly.destroy()
        }
    }

    ParallelAnimation {
        id: flyAnimation
        running: false
        NumberAnimation {
            id: xmove
            target: fly
            property: "x"
            duration: 500
            easing.type: Easing.InOutQuad
        }
        NumberAnimation {
            id: ymove
            target: fly
            property: "y"
            duration: 500
            easing.type: Easing.InOutQuad
        }
        /*SequentialAnimation {
            NumberAnimation {
                target: fly
                property: "opacity"
                to: 1.0
                duration: 100
                easing.type: Easing.InOutQuad
            }
            PauseAnimation {
                duration: 300
            }
            NumberAnimation {
                target: fly
                property: "opacity"
                to: 0.0
                duration: 100
            }
        }*/
    }

}
