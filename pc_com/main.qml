import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "PyQt6 QML Interface"

    Column {
        spacing: 10
        anchors.centerIn: parent
        width: parent.width

        // Source file selection
        TextField {
            width: parent.width
            id: sourceField
            placeholderText: "Select source file"
            MouseArea {
                anchors.fill: parent
                onClicked: sourceDialog.open()
            }
        }
        FileDialog {
            id: sourceDialog
            onAccepted: {
                // Using fileUrl for a single selected file
                var sourcePath = sourceDialog.currentFile
                console.log("source path: ", sourcePath)
                sourceField.text = sourcePath // Update the TextField with the selected file path
            }
        }
        // Destination file selection
        TextField {
            id: destinationField
            width: parent.width
            placeholderText: "Select destination file"
        }

        // ComboBox for commands
        ComboBox {
            id: commandBox
            model: ["WRITE", "SHOW", "DELETE", "DIR"]
        }

        // Make Image Button
        Button {
            text: "MakeImage"
            onClicked: backend.makeImage(sourceField.text, destinationField.text)
        }

        // Send Command Button
        Button {
            text: "SendCommand"
            onClicked: backend.sendCommand(commandBox.currentText, destinationField.text)
        }
    }
}
