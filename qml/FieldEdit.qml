import QtQuick 2.12

TableView {
    id: tableView

    //anchors.fill: parent
    //anchors.margins: 5
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
    delegate: Rectangle {
        property int size: 10
        border.width: 1
        onWidthChanged: {
            var n = Math.ceil(Math.sqrt(sizen));
            size = (width - 2) / n - 2;
        }

        Flow {
            anchors.fill: parent
            anchors.margins: 1
            spacing: 2
            Repeater {
                model: editCell
                delegate: Fly {
                    dead: false
                    width: size
                    height: size
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                var f  = editCell;
                if (mouse.button === Qt.RightButton) {
                    if (f.length === 0)
                        return;
                    f.pop();
                    editCell = f;
                } else if (mouse.button === Qt.LeftButton) {
                    console.log(f.length)
                    if (f.length >= sizen)
                        return;
                    f.push(slider.value);
                    editCell = f;
                }
            }
        }
    }

}
