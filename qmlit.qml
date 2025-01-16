import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "My QML App"

    Column {
        anchors.centerIn: parent
        spacing: 10

        Text {
            text: "Hello, QML!"
            font.pixelSize: 24
        }

        Button {
            text: "Click Me"
            onClicked: {
                console.log("Button clicked!");
            }
        }
    }
}