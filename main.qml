
import QtQuick 2.12
import litovko 1.0
import QtQuick.Controls 2.12
//import QtCharts 2.2

ApplicationWindow {
    visible: true
    width: 1500
    height: 900
    title: qsTr("Hello World")
    color: "transparent"

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 1
        border.color: "yellow"
        border.width: 1
        radius: 5
        focus: true
        Keys.onPressed: {
            print("key " + event.key)
            if (event.key === Qt.Key_F2) {

                f.load_file("d:/morse2.wav")
            }
        }
        Rectangle{
            id: p
            width: 600;
            height: 100;
            color: "#000000"
            radius: 3
            border.width: 1
            border.color: "yellow"
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                margins: 5
            }

            TextField {
                id: tf
                height: 30
                anchors.leftMargin: 300
                anchors.topMargin: 8
                anchors {top:parent.top; left: parent.left;margins: 10}
                text: f.frame_start.toFixed(2)
            }

            Button {
                id: button
                x: 32
                y: 8
                text: qsTr("fft")
                onClicked: {
                    f.frame_start=tf.text
                    f.frame_width=tw.text
                    f.fft()
                }
            }

            TextField {
                id: tw
                x: 0
                y: -5
                height: 30
                anchors.leftMargin: 300
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 10
                anchors.topMargin: 35
                text: f.frame_width.toFixed(2)
            }

            TextField {
                id: step
                x: 0
                y: -1
                height: 30
                text: "0.01"
                anchors.left: parent.left
                anchors.topMargin: 62
                anchors.leftMargin: 300
                anchors.top: parent.top
            }

            Button {
                id: b_left
                x: 99
                y: 57
                text: qsTr("<<")
                onClicked: {
                    if (Number(tf.text)>=Number(step.text)) {
                        tf.text=(Number(tf.text)-Number(step.text)).toFixed(3)
                        f.frame_start=tf.text
                        f.frame_width=tw.text
                        f.fft()
                    }


                }
            }

            Button {
                id: b_right
                x: 200
                y: 57
                text: qsTr(">>")
                onClicked: {
                    tf.text=(Number(tf.text)+Number(step.text)).toFixed(3)
                    f.frame_start=tf.text
                    f.frame_width=tw.text
                    f.fft()
                }
            }

        }
    }
    Rectangle {
        id: plot_area
        border.width: 1
        border.color: "yellow"
        radius: 5
        color: "transparent"
        height: 650
        anchors {
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                    margins: 50
        }
        PlotChart {
            id: plotc
            height: parent.height/2-2*anchors.margins
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                margins: 5
            }
        }
        PlotChart {
            id: plotf
            height: parent.height/2-2*anchors.margins
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: 5
            }
        }
    }

    WavFile{
        id: f
        chart: plotc.plot
        chartf: plotf.plot
    }
}
