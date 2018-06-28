import QtQuick 2.11
import QtQuick.Window 2.11
import litovko 1.0
import QtQuick.Dialogs 1.2
import QtCharts 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    color: "transparent"

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.5
        border.color: "yellow"
        border.width: 3
        radius: 5
    }
    ChartView {
        id: cv
        title: "Line"
        anchors.fill: parent
        antialiasing: true


        Rectangle {
            id: marker
            height: ma.height
            width: x-ma.mouseX
            color: "transparent"
            radius: 1
            border.color: "gray"
        }
//        MouseArea {
//            id: ma
//            //preventStealing: true
//            propagateComposedEvents: true
//            anchors.fill: parent
//            onPressed: console.log("onPressed: " + ma.mouseX + ", " + ma.mouseY);
//        }

        LineSeries {
            id: source
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
    WavFile{
        id: f
        source:  source
        Component.onCompleted:  f.load_file("d:/morse2.wav")
    }
//    FileDialog {

//    }

}
