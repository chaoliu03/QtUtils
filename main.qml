import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("QtUtils")

    ScrollView {
        anchors.fill: parent

        TextArea {
            id: ta_log
            font.pixelSize: 14
            placeholderText: ""
            selectByMouse: true
            // readOnly: true
        }

        Connections {
            target: logMgr
            function onSigSendLogMsg(msg) {
                if (ta_log.lineCount > 25) {
                    var remove_index = ta_log.text.indexOf('\n')
                    ta_log.remove(0, remove_index + 1)
                }

                ta_log.append(msg)
            }
        }
    }

    Component.onCompleted: {
        console.log("QML测试LOG")
    }
}
