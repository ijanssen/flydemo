import QtQuick 2.12
import QtQuick.Controls 2.5

TableView {
    id: tableView

    anchors.fill: parent
    anchors.margins: 5
    columnSpacing: 0
    rowSpacing: 0
    clip: true

    columnWidthProvider: function () {
        return size;
    }
    rowHeightProvider: function () {
        return size;
    }

    property int size

    function doLayout() {
        size = tableView.width / tableView.model.columnCount();
        forceLayout()
    }

    onRowsChanged: doLayout()
    onColumnsChanged: doLayout()
    onWidthChanged: doLayout()
    onHeightChanged: doLayout()



    model: fieldModel
    Connections {
        target: fieldModel
        function onMoved(xcell, ycell, nxcell, nycell) {
            // Move animation
            var obj = flyMoving.createObject(tableView.contentItem);
            if (obj === null) {
                console.log("error!");
                console.log(flyMoving.errorString());
                return;
            }

            obj.width = tableView.size / 2;
            obj.height = tableView.size / 2;
            obj.x = xcell * tableView.size + obj.width / 2;
            obj.y = ycell * tableView.size + obj.height / 2;
            var nx = nxcell * tableView.size + obj.width / 2;
            var ny = nycell * tableView.size + obj.height / 2;
            obj.startMove(nx, ny);
        }
    }

    delegate: Rectangle {
        property int size: 10
        border.width: 1
        onWidthChanged: {
            // Minimum square to place N cells
            var n = Math.ceil(Math.sqrt(sizen));
            size = (width - 2) / n - 2;
        }

        Flow {
            anchors.fill: parent
            anchors.margins: 1
            spacing: 2
            Repeater {
                model: fcount
                delegate: Fly {
                    dead: modelData[0]
                    t: modelData[1]
                    width: size
                    height: size
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            //const d = modelData[0] ? "yes" : "no";
                            const t = qsTr("Dead") + ": " + (modelData[0] ? "yes" : "no") + "\n" +
                                    "T: " + modelData[1] + "\n" +
                                    qsTr("Steps") + ": " + modelData[2] + "\n" +
                                    qsTr("Life time") + ": " + modelData[3] / 1000 + qsTr(" secs") + "\n" +
                                    qsTr("Speed") + ": " + (modelData[3] > 0 ? (1000 * modelData[2] / modelData[3]) : 0) + qsTr(" steps/sec")
                            var x = parent.parent.parent.x
                            if (y < tableView.contentItem.height / 2)
                                message.showMessage(x, parent.parent.parent.y + parent.parent.parent.height, t)
                            else
                                message.showMessage(x, parent.parent.parent.y, t, true)
                        }
                    }
                }
            }
        }
    }

    Message {
        id: message
        z : 100
    }

    Component {
        id: flyMoving
        FlyMoving {
            width: 100
            height: 100
            z: 10
        }
    }
}
