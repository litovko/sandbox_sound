import QtQuick 2.9
import QtQuick.Window 2.3
import litovko 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.2
import QtCharts 2.2

Window {
    visible: true
    width: 1000
    height: 800
    title: qsTr("Hello World")
    color: "transparent"

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 1
        border.color: "yellow"
        border.width: 3
        radius: 5
        Rectangle{
            id: p
            width: 500;
            height: 100;
            color: "#000000"
            radius: 3
            border.width: 4
            border.color: "#515224"
            TextField {
                id: tf
                height: 30
                anchors.leftMargin: 300
                anchors.topMargin: 8
                anchors {top:parent.top; left: parent.left;margins: 10}
                text: f.frame_start.toFixed(1)
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
                text: f.frame_width.toFixed(1)
            }

            TextField {
                id: step
                x: 0
                y: -1
                height: 30
                text: "0.1"
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
    ChartView {
        id: cv
        title: "Line"
        anchors.top: parent.top
        anchors.topMargin: p.height
        anchors.horizontalCenter: parent.horizontalCenter
        height: 300
        width: 900
        antialiasing: true

        LineSeries {
            id: src
            name: "LineSeries"
            color: "black"
            XYPoint { x: 0; y: 0 }
            XYPoint { x: 1.1; y: -1 }

            XYPoint { x: 7; y: 1 }
            onClicked: console.log("onClicked: " + point.x + ", " + point.y);

                onPressed: {
                  console.log(" mx:"+point.x+", "+ point.x)
                  console.log(cv.mapToPosition(point));


                          }
                onReleased: {

                  console.log(" released:"+point.x)

                          }

        }
    }
    ChartView {
        id: cvres
        title: "Line"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: 300
        width: 900
        antialiasing: true

        LineSeries {
            id: res
            name: "Результаты"
            color: "black"
            XYPoint { x: 0; y: 0 }
            XYPoint { x: 1.1; y: 20 }

            XYPoint { x: 48000; y: 1 }

        }
    }
    WavFile{
        id: f
        source:  src
        result: res
        Component.onCompleted:  f.load_file("d:/morse2.wav")
    }
//    FileDialog {

//    }

}
